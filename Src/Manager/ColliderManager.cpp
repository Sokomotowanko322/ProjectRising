#include <DxLib.h>
#include "../Utility/CollisionUtility.h"
#include "../Object/Unit/Player.h"
#include "../Object/Unit/NormalEnemy.h"
#include "../Object/Weapon.h"
#include "ColliderManager.h"

// プレイヤー攻撃アニメ種別→敵のリアクション
static const std::unordered_map<Player::ANIM_TYPE, std::function<void(NormalEnemy*, const VECTOR&)>> reactionTable_ = 
{
    { Player::ANIM_TYPE::SMASH,      [](NormalEnemy* e, const VECTOR& dir) { e->ChangeState(NormalEnemy::STATE::FLINCH); } },
    { Player::ANIM_TYPE::FIRST_COMBO,[](NormalEnemy* e, const VECTOR& dir) { e->ChangeState(NormalEnemy::STATE::FLINCH); } },
    { Player::ANIM_TYPE::HIGHTIME,   [](NormalEnemy* e, const VECTOR& dir) { e->ChangeState(NormalEnemy::STATE::FLINCH); } },
};

void ColliderManager::RegisterActor(const std::shared_ptr<ActorBase>& actor)
{
    actors_.push_back(actor);
}

void ColliderManager::DrawColliders()
{
    int idx = 0;
    for (const auto& col : colliders_)
    {
        printfDx("Collider[%d]: type=%d, ownerID=%d, pos=(%.2f, %.2f, %.2f)\n",
            idx, (int)col.type_, col.ownerID_, col.pos_.x, col.pos_.y, col.pos_.z);
        ++idx;

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
        //else if (col.type_ == ColliderType::StageTransform)
        //{
        //    // ボックス（AABB）描画例
        //    VECTOR min = VSub(col.pos_, VScale(col.dir_, 0.5f));
        //    VECTOR max = VAdd(col.pos_, VScale(col.dir_, 0.5f));
        //    DrawBox(max.x, max.y, max.z,GetColor(0, 255, 0),true,1);
        //}
        //// 必要に応じて他のタイプも追加
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

            // --- ここでコライダーの位置をアクター本体に反映 ---
            for (auto& col : colliders_)
            {
                // 右手コライダーは除外
                if (col.type_ == ColliderType::Capsule &&
                    col.ownerID_ == actor->GetTransform().modelId &&
                    !col.isRightHand_)
                {
                    actor->SetPos(col.pos_);
                }
            }
            ++i;
        }
        else
        {
            i = actors_.erase(i);
        }
    }
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

        for (auto& col : colliders_)
        {
            if (col.type_ != ColliderType::Capsule) continue;

            if (weapon && col.ownerID_ == weapon->GetWeaponTransform().modelId) {
                // デバッグ出力
                printfDx("UpdateColliders: col.ownerID_=%d, weaponModelId=%d\n", col.ownerID_, weapon->GetWeaponTransform().modelId);

                // 武器コライダの更新
                const Transform& trans = weapon->GetWeaponTransform();
                VECTOR scl = trans.scl;
                VECTOR pos = trans.pos;
                Quaternion rot = trans.quaRot;

                VECTOR tipWorld = VAdd(pos, Quaternion::PosAxis(rot, { 1200.0f * scl.x, 0.0f, 0.0f }));
                tipWorld.x += 30.0f;
                tipWorld.y += 16.0f;
                VECTOR baseWorld = VAdd(pos, Quaternion::PosAxis(rot, { 0.0f, 0.0f, 0.0f }));

                VECTOR center = VScale(VAdd(tipWorld, baseWorld), 0.5f);
                VECTOR dir = VNorm(VSub(tipWorld, baseWorld));
                float height = VSize(VSub(tipWorld, baseWorld));

                col.pos_ = center;
                col.dir_ = dir;
                col.length_ = height;
            }
            else if (player && col.isRightHand_ && col.ownerID_ == actorId) {
                col.pos_ = player->GetRightHandPos();
            }
            else if (col.ownerID_ == actorId) {
                col.pos_ = actor->GetPos();
            }
        }
    }
}

void ColliderManager::CheckCollisions() {
    for (size_t i = 0; i < colliders_.size(); ++i) {
        for (size_t j = i + 1; j < colliders_.size(); ++j) {
            ColliderData& a = colliders_[i];
            ColliderData& b = colliders_[j];

            if (a.type_ == ColliderType::Capsule && b.type_ == ColliderType::Capsule) 
            {
                if (!a.isTrigger_ && !b.isTrigger_)
                {
                    ResolveCapsuleCollision(a, b); // 追加：自機と敵など
                }
               
            }

            // aが攻撃側（Weapon）、bが敵（NormalEnemy）の場合
            ActorBase* actorA = nullptr;
            ActorBase* actorB = nullptr;
            for (auto& weakActor : actors_) {
                if (auto actor = weakActor.lock()) {
                    if (actor->GetTransform().modelId == a.ownerID_) actorA = actor.get();
                    if (actor->GetTransform().modelId == b.ownerID_) actorB = actor.get();
                }
            }
            Weapon* weapon = dynamic_cast<Weapon*>(actorA);
            NormalEnemy* enemy = dynamic_cast<NormalEnemy*>(actorB);
            if (weapon && enemy) {
                // Playerを探して攻撃中か判定
                for (auto& weakActor : actors_) {
                    if (auto actor = weakActor.lock()) {
                        Player* player = dynamic_cast<Player*>(actor.get());
                        if (player && player->GetWeapon().get() == weapon && player->IsAttack()) {
                            HitAttackToDamage(a, b);
                            break;
                        }
                    }
                }
            }


            if (a.type_ == ColliderType::Capsule && b.type_ == ColliderType::StageTransform) {
                ResolveStageCollision(a, b);
            }
            else if (b.type_ == ColliderType::Capsule && a.type_ == ColliderType::StageTransform) {
                ResolveStageCollision(b, a);
            }
        }
    }
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
    // 2つのカプセルの中心座標の差分ベクトル（法線）
    VECTOR dir = VSub(a.pos_, b.pos_);
    float dist = VSize(dir);

    // 半径の合計
    float rSum = a.radius_ + b.radius_;

    // めり込み量を計算
    float penetration = rSum - dist;

    if (penetration > 0.0f && dist > 1e-4f) {
        // 法線を正規化
        VECTOR normal = VNorm(dir);

        // 少しだけ余裕を持たせて押し出す
        float pushBack = penetration + 0.05f;

        // 両者を均等に押し出す
        a.pos_ = VAdd(a.pos_, VScale(normal, pushBack * 20.0f));
        b.pos_ = VAdd(b.pos_, VScale(normal, -pushBack * 20.0f));
    }
}

void ColliderManager::HitAttackToDamage(const ColliderData& self, const ColliderData& target) 
{
    // ownerIDからアクターを特定
    ActorBase* attacker = nullptr;
    ActorBase* victim = nullptr;
    for (auto& weakActor : actors_)
    {
        if (auto actor = weakActor.lock()) 
        {
            if (actor->GetTransform().modelId == self.ownerID_) 
            {
                attacker = actor.get();
            }
            if (actor->GetTransform().modelId == target.ownerID_) 
            {
                victim = actor.get();
            }
        }
    }
    if (!attacker || !victim) 
    {
        return;
    }
    // Weapon→NormalEnemy の場合
    if (auto weapon = dynamic_cast<Weapon*>(attacker)) {
        if (auto enemy = dynamic_cast<NormalEnemy*>(victim)) {
            // Playerを探す
            for (auto& weakActor : actors_) {
                if (auto actor = weakActor.lock()) {
                    auto player = dynamic_cast<Player*>(actor.get());
                    if (player && player->GetWeapon().get() == weapon && player->IsAttack()) {
                        enemy->Damage(1); // ダメージ量は適宜
                        return;
                    }
                }
            }
        }
    }
    // プレイヤー攻撃→敵の場合のみ判定をする
    Player* player = dynamic_cast<Player*>(attacker);
    NormalEnemy* enemy = dynamic_cast<NormalEnemy*>(victim);
    if (player && enemy) 
    {
        auto animType = player->GetCurrentAnimType();
        auto it = reactionTable_.find(animType);
        if (it != reactionTable_.end())
        {
            VECTOR dir = VNorm(VSub(enemy->GetPos(), player->GetPos()));
            it->second(enemy, dir); // 関数テーブルでリアクション実行
        }
    }
}