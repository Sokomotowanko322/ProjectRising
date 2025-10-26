#include <DxLib.h>
#include "../Utility/CollisionUtility.h"
#include "../Object/Unit/Player.h"
#include "../Object/Unit/MidBoss.h"
#include "../Object/Weapon.h"
#include "ColliderManager.h"

// プレイヤー攻撃アニメ種別→敵のリアクション
static const std::unordered_map<Player::ANIM_TYPE, std::function<void(MidBoss*, const VECTOR&)>> reactionTable_ = 
{
    { Player::ANIM_TYPE::SMASH,      [](MidBoss* e, const VECTOR& dir) { e->ChangeState(MidBoss::STATE::BLOW_AWAY); } },
    { Player::ANIM_TYPE::FIRST_COMBO,[](MidBoss* e, const VECTOR& dir) { e->ChangeState(MidBoss::STATE::FLINCH); } },
    { Player::ANIM_TYPE::HIGHTIME,   [](MidBoss* e, const VECTOR& dir) { e->ChangeState(MidBoss::STATE::BLOW); } },
};

void ColliderManager::RegisterActor(const std::shared_ptr<ActorBase>& actor)
{
	// それぞれの情報を登録
    actors_.push_back(actor);
}

void ColliderManager::DrawColliders()
{
    int idx = 0;
    for (const auto& col : colliders_)
    {
        if (col.type_ == ColliderType::Capsule)
        {
            // カプセルの中心col.pos_、方向col.dir_、高さcol.height_、半径col.radius_の場合
            VECTOR start = VAdd(col.pos_, VScale(col.dir_, -col.length_ * 0.5f));
            VECTOR end = VAdd(col.pos_, VScale(col.dir_, col.length_ * 0.5f));
            DrawCapsule3D(
                start,
                end,
                col.radius_,
                16,                        // 分割数
                GetColor(255, 0, 0),         // 側面の色
                GetColor(255, 0, 0),         // 両端の色
                FALSE                      // ワイヤーフレーム
            );
        }
    }
}

void ColliderManager::AddCollider(const ColliderData& collider)
{
    colliders_.push_back(collider);
}

void ColliderManager::Update() 
{
    for (auto i = actors_.begin(); i != actors_.end(); )
    {
        if (auto actor = i->lock())
        {
            UpdateColliders();
            CheckCollisions();
            ++i;
        }
        else
        {
            i = actors_.erase(i);
        }
    }
}

bool ColliderManager::IsWeaponEnemyPair(const ColliderData& weaponCol, const ColliderData& enemyCol)
{
    ActorBase* weaponActor = nullptr;
    ActorBase* enemyActor = nullptr;
    for (auto& weakActor : actors_) {
        if (auto actor = weakActor.lock()) {
            // Weaponの判定はweaponTransformのmodelIdで行う
            if (auto weapon = dynamic_cast<Weapon*>(actor.get())) {
                if (weapon->GetWeaponTransform().modelId == weaponCol.ownerID_) {
                    weaponActor = actor.get();
                }
            }
            // Enemyは従来通り
            if (actor->GetTransform().modelId == enemyCol.ownerID_) {
                enemyActor = actor.get();
            }
        }
    }
    return dynamic_cast<Weapon*>(weaponActor) && dynamic_cast<MidBoss*>(enemyActor);
}

void ColliderManager::UpdateColliders()
{
    for (auto& weakActor : actors_)
    {
        auto actor = weakActor.lock();
        if (!actor) continue;

        int actorId = actor->GetTransform().modelId;
        Player* player = dynamic_cast<Player*>(actor.get());
        Weapon* weapon = dynamic_cast<Weapon*>(actor.get());
        MidBoss* enemy = dynamic_cast<MidBoss*>(actor.get());

        for (auto& col : colliders_)
        {
            if (col.type_ != ColliderType::Capsule) continue;

            // 武器コライダの完全追従
            if (weapon && col.ownerID_ == weapon->GetWeaponTransform().modelId) {
                const Transform& trans = weapon->GetWeaponTransform();
                VECTOR scl = trans.scl;
                VECTOR pos = trans.pos;
                Quaternion rot = trans.quaRot;

                VECTOR handleBaseLocal = { 0.0f, 0.0f, 0.0f };
                VECTOR bladeTipLocal = { 1200.0f, 0.0f, 0.0f };

                // スケール適用
                handleBaseLocal.x *= scl.x;
                handleBaseLocal.y *= scl.y;
                handleBaseLocal.z *= scl.z;
                bladeTipLocal.x *= scl.x;
                bladeTipLocal.y *= scl.y;
                bladeTipLocal.z *= scl.z;

                VECTOR baseWorld = VAdd(pos, Quaternion::PosAxis(rot, handleBaseLocal));
                VECTOR tipWorld = VAdd(pos, Quaternion::PosAxis(rot, bladeTipLocal));

                VECTOR center = VScale(VAdd(tipWorld, baseWorld), 0.5f);
                VECTOR dir = VNorm(VSub(tipWorld, baseWorld));
                float height = VSize(VSub(tipWorld, baseWorld));

                col.pos_ = center;
                col.dir_ = dir;
                col.length_ = height;
                continue;
            }
            // 2. プレイヤー右手コライダー
            if (player && col.isRightHand_ && col.ownerID_ == player->GetTransform().modelId) {
                col.pos_ = player->GetRightHandPos();
                continue;
            }
            // プレイヤー本体コライダー
            if (player && !col.isRightHand_ && col.ownerID_ == player->GetTransform().modelId) {
                // モデル原点＋Yオフセット
                constexpr float PLAYER_WAIST_OFFSET_Y = 50.0f; // 実際のモデルに合わせて調整
                VECTOR pos = player->GetPos();
                pos.y += PLAYER_WAIST_OFFSET_Y;
                col.pos_ = pos;
                continue;
            }
            // 敵本体コライダー
            if (enemy && col.ownerID_ == enemy->GetTransform().modelId) {
                constexpr float ENEMY_WAIST_OFFSET_Y = 50.0f; // 敵モデルに合わせて調整
                VECTOR pos = enemy->GetPos();
                pos.y += ENEMY_WAIST_OFFSET_Y;
                col.pos_ = pos;
                continue;
            }
        }
    }
}

void ColliderManager::CheckCollisions() {
    // プレイヤー攻撃開始検知（ColliderManager内で完結）
    static bool prevPlayerAttack = false;
    Player* player = nullptr;
    for (auto& weakActor : actors_) {
        if (auto actor = weakActor.lock()) {
            player = dynamic_cast<Player*>(actor.get());
            if (player) break;
        }
    }
    bool nowAttack = player && player->IsAttack();
    if (nowAttack && !prevPlayerAttack) {
        ResetHitCount();
    }
    prevPlayerAttack = nowAttack;

    for (size_t i = 0; i < colliders_.size(); ++i) {
        for (size_t j = i + 1; j < colliders_.size(); ++j) {
            ColliderData& a = colliders_[i];
            ColliderData& b = colliders_[j];

            // 本体同士の物理衝突
            if (a.type_ == ColliderType::Capsule && b.type_ == ColliderType::Capsule &&
                !a.isTrigger_ && !b.isTrigger_) {
                ResolveCapsuleCollision(a, b);
                continue;
            }

            // --- 武器コライダと敵コライダの攻撃判定 ---
            if (a.type_ == ColliderType::Capsule && a.isTrigger_ &&
                b.type_ == ColliderType::Capsule && !b.isTrigger_ &&
                IsWeaponEnemyPair(a, b)) {

                VECTOR wStart = VAdd(a.pos_, VScale(a.dir_, -a.length_ * 0.5f));
                VECTOR wEnd = VAdd(a.pos_, VScale(a.dir_, a.length_ * 0.5f));
                VECTOR eStart = VAdd(b.pos_, VScale(b.dir_, -b.length_ * 0.5f));
                VECTOR eEnd = VAdd(b.pos_, VScale(b.dir_, b.length_ * 0.5f));
                float minDist = CollisionUtility::GetSegmentSegmentDistance(wStart, wEnd, eStart, eEnd);

                if (minDist <= a.radius_ + b.radius_) {
                    Weapon* weapon = nullptr;
                    for (auto& weakActor : actors_) {
                        if (auto actor = weakActor.lock()) {
                            weapon = dynamic_cast<Weapon*>(actor.get());
                            if (weapon && weapon->GetWeaponTransform().modelId == a.ownerID_) break;
                        }
                    }
                    if (!weapon) continue;
                    Player* player = FindPlayerByWeapon(weapon);
                    if (player && player->IsAttack()) {
                        ActorBase* victim = nullptr;
                        for (auto& weakActor : actors_) {
                            if (auto actor = weakActor.lock()) {
                                if (actor->GetTransform().modelId == b.ownerID_) {
                                    victim = actor.get();
                                    break;
                                }
                            }
                        }
                        if (auto enemy = dynamic_cast<MidBoss*>(victim)) {
                            int enemyId = enemy->GetTransform().modelId;
                            if (hitCount_ < 20 && hitEnemyIds_.count(enemyId) == 0) {
                                HitAttackToDamage(a, b, player);
                                hitEnemyIds_.insert(enemyId);
                                ++hitCount_;
                            }
                        }
                    }
                }
            }
            // bが武器コライダ、aが敵コライダの場合（逆も判定）
            else if (b.type_ == ColliderType::Capsule && b.isTrigger_ &&
                a.type_ == ColliderType::Capsule && !a.isTrigger_ &&
                IsWeaponEnemyPair(b, a)) {

                VECTOR wStart = VAdd(b.pos_, VScale(b.dir_, -b.length_ * 1.4f));
                VECTOR wEnd = VAdd(b.pos_, VScale(b.dir_, b.length_ * 1.4f));
                VECTOR eStart = VAdd(a.pos_, VScale(a.dir_, -a.length_ * 1.4f));
                VECTOR eEnd = VAdd(a.pos_, VScale(a.dir_, a.length_ * 1.4f));
                float minDist = CollisionUtility::GetSegmentSegmentDistance(wStart, wEnd, eStart, eEnd);

                if (minDist <= b.radius_ + a.radius_) {
                    Weapon* weapon = nullptr;
                    for (auto& weakActor : actors_) {
                        if (auto actor = weakActor.lock()) {
                            weapon = dynamic_cast<Weapon*>(actor.get());
                            if (weapon && weapon->GetWeaponTransform().modelId == b.ownerID_) break;
                        }
                    }
                    if (!weapon) continue;
                    Player* player = FindPlayerByWeapon(weapon);
                    if (player && player->IsAttack()) {
                        ActorBase* victim = nullptr;
                        for (auto& weakActor : actors_) {
                            if (auto actor = weakActor.lock()) {
                                if (actor->GetTransform().modelId == a.ownerID_) {
                                    victim = actor.get();
                                    break;
                                }
                            }
                        }
                        if (auto enemy = dynamic_cast<MidBoss*>(victim)) {
                            int enemyId = enemy->GetTransform().modelId;
                            if (hitCount_ < 20 && hitEnemyIds_.count(enemyId) == 0) {
                                HitAttackToDamage(b, a, player);
                                hitEnemyIds_.insert(enemyId);
                                ++hitCount_;
                            }
                        }
                    }
                }
            }

            // ステージとの衝突
            if (a.type_ == ColliderType::Capsule && b.type_ == ColliderType::StageTransform) {
                CheckStageMeshCollision(a, b.ownerID_);
                continue;
            }
            if (b.type_ == ColliderType::Capsule && a.type_ == ColliderType::StageTransform) {
                CheckStageMeshCollision(b, a.ownerID_);
                continue;
            }
        }
    }
}

void ColliderManager::CheckStageMeshCollision(ColliderData& capsuleCol, int stageModelId)
{
    // カプセルの端点を計算
    VECTOR capStart = VAdd(capsuleCol.pos_, VScale(capsuleCol.dir_, -capsuleCol.length_ * 0.5f));
    VECTOR capEnd = VAdd(capsuleCol.pos_, VScale(capsuleCol.dir_, capsuleCol.length_ * 0.5f));

    MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Capsule(
        stageModelId, -1,
        capStart, capEnd, capsuleCol.radius_
    );

    constexpr int PUSH_TRY_COUNT = 5;
    constexpr float PUSH_FORCE = 0.05f;

    VECTOR totalDelta = VGet(0, 0, 0);

    for (int i = 0; i < hits.HitNum; ++i) {
        const auto& hit = hits.Dim[i];

        for (int tryCnt = 0; tryCnt < PUSH_TRY_COUNT; ++tryCnt) {
            VECTOR oldPos = capsuleCol.pos_;
            capsuleCol.pos_ = VAdd(capsuleCol.pos_, VScale(hit.Normal, PUSH_FORCE));
            VECTOR delta = VSub(capsuleCol.pos_, oldPos);
            totalDelta = VAdd(totalDelta, delta);
        }
        // Y成分が大きい場合は接地判定
        if (hit.Normal.y > 0.7f) {
            for (auto& weakActor : actors_) {
                if (auto player = dynamic_cast<Player*>(weakActor.lock().get())) {
                    if (player->GetTransform().modelId == capsuleCol.ownerID_) {
                        player->isGrounded_ = true;
                    }
                }
            }
        }
    }

    // コライダーのownerIDと一致するアクター本体も同じだけ移動
    if (totalDelta.x != 0.0f || totalDelta.y != 0.0f || totalDelta.z != 0.0f) {
        for (auto& weakActor : actors_) {
            if (auto actor = weakActor.lock()) {
                if (actor->GetTransform().modelId == capsuleCol.ownerID_) {
                    VECTOR pos = actor->GetPos();
                    pos = VAdd(pos, totalDelta);
                    actor->SetPos(pos);
                }
            }
        }
    }

    MV1CollResultPolyDimTerminate(hits);
}

Player* ColliderManager::FindPlayerByWeapon(Weapon* weapon)
{
    for (auto& weakActor : actors_) {
        if (auto actor = weakActor.lock()) {
            auto player = dynamic_cast<Player*>(actor.get());
            if (player && player->GetWeapon().get() == weapon) {
                return player;
            }
        }
    }
    return nullptr;
}

void ColliderManager::ResolveStageCollision(ColliderData& mover, const ColliderData& stage)
{
    // ステージAABBの中心・サイズ（pos_が中心、dir_がサイズと仮定）
    VECTOR boxCenter = stage.pos_;
    VECTOR boxSize = stage.dir_;

    // カプセルの中心
    VECTOR capsuleCenter = mover.pos_;

    // 最近接点をAABB上に求める
    VECTOR closest;
    closest.x = (std::max)(boxCenter.x - boxSize.x * 0.5f, (std::min)(capsuleCenter.x, boxCenter.x + boxSize.x * 0.5f));
    closest.y = (std::max)(boxCenter.y - boxSize.y * 0.5f, (std::min)(capsuleCenter.y, boxCenter.y + boxSize.y * 0.5f));
    closest.z = (std::max)(boxCenter.z - boxSize.z * 0.5f, (std::min)(capsuleCenter.z, boxCenter.z + boxSize.z * 0.5f));

    // 法線ベクトル（AABB表面→カプセル中心方向）
    VECTOR normal = VSub(capsuleCenter, closest);
    float len = VSize(normal);

    if (len < 1e-4f) {
        // 法線が求まらない場合は上方向
        normal = VGet(0, 1, 0);
    }
    else {
        normal = VNorm(normal);
    }

    // 押し出し量（カプセルがめり込んでいる分＋少し余裕）
    float penetration = mover.radius_ - len;
    if (penetration > 0.0f) {
        mover.pos_ = VAdd(mover.pos_, VScale(normal, penetration + 0.01f));
    }
}

void ColliderManager::ResolveCapsuleCollision(ColliderData& a, ColliderData& b)
{
    VECTOR dir = VSub(a.pos_, b.pos_);
    float dist = VSize(dir);
    float rSum = a.radius_ + b.radius_;
    float penetration = rSum - dist;

    if (penetration > 0.0f && dist > 1e-4f) {
        VECTOR normal = VNorm(dir);
        float pushBack = penetration + 0.05f;

        // 押し出し前の位置を保存
        VECTOR oldA = a.pos_;
        VECTOR oldB = b.pos_;

        // コライダーを押し出す
        a.pos_ = VAdd(a.pos_, VScale(normal, pushBack * 1.0f));
        b.pos_ = VAdd(b.pos_, VScale(normal, -pushBack * 1.0f));

        // 押し出し量を計算
        VECTOR deltaA = VSub(a.pos_, oldA);
        VECTOR deltaB = VSub(b.pos_, oldB);

        // アクター本体も同じだけ移動させる
        for (auto& weakActor : actors_) {
            if (auto actor = weakActor.lock()) {
                // a側
                if (actor->GetTransform().modelId == a.ownerID_) {
                    VECTOR pos = actor->GetPos();
                    pos = VAdd(pos, deltaA);
                    actor->SetPos(pos);
                }
                // b側
                if (actor->GetTransform().modelId == b.ownerID_) {
                    VECTOR pos = actor->GetPos();
                    pos = VAdd(pos, deltaB);
                    actor->SetPos(pos);
                }
            }
        }
    }
}

// プレイヤーを引数で受け取る形に変更
void ColliderManager::HitAttackToDamage(const ColliderData& self, const ColliderData& target, Player* player)
{
    ActorBase* victim = nullptr;
    for (auto& weakActor : actors_) {
        if (auto actor = weakActor.lock()) {
            if (actor->GetTransform().modelId == target.ownerID_) {
                victim = actor.get();
                break;
            }
        }
    }
    if (!victim || !player) return;

    if (auto enemy = dynamic_cast<MidBoss*>(victim)) 
    {
        // リアクションテーブル実行
        auto animType = player->GetCurrentAnimType();
        auto it = reactionTable_.find(animType);
        if (it != reactionTable_.end()) {
            VECTOR dir = VNorm(VSub(enemy->GetPos(), player->GetPos()));
            player->HitEffect(enemy->GetPos());
            // ヒットストップ・カメラシェイク
            player->HitStop(0.2f); // ← Playerに通知
            // 攻撃種別ごとにスローモーション
            if (animType == Player::ANIM_TYPE::SMASH) {
                player->StartSlow(0.3f, 0.2f);
            }
            else if (animType == Player::ANIM_TYPE::HIGHTIME) {
                player->StartSlow(0.2f, 0.3f);
            }
            it->second(enemy, dir);
        }
        enemy->Damage(2);
    }
}

void ColliderManager::ResetHitCount()
{
    hitEnemyIds_.clear();
    hitCount_ = 0;
}