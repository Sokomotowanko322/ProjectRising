#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include "../Collider/ColliderData.h"
#include "../Object/Unit/ActorBase.h"

class ActorBase;
class Weapon;

class ColliderManager 
{
public:

    // アクターを登録
    void RegisterActor(const std::shared_ptr<ActorBase>& actor);

    // デバッグ描画
    void DrawColliders();

	// コライダーを追加
    void AddCollider(const ColliderData& collider);
    
    // まとめて更新
    void Update();

	// 衝突判定を行う
    bool IsWeaponEnemyPair(const ColliderData& weaponCol, const ColliderData& enemyCol);

    // リセット関数
    void ResetHitCount();


private:

    // 複数アクターを管理
    std::vector<std::weak_ptr<ActorBase>> actors_; 

	// コライダーのリスト
    std::vector<ColliderData> colliders_;
   
    // コライダの位置などを更新する
    void UpdateColliders();
	
    // 当たり判定をチェック
    void CheckCollisions();

    // ステージ用当たり判定チェック関数
    void CheckStageMeshCollision(ColliderData& capsuleCol, int stageModelId);
    
    Player* FindPlayerByWeapon(Weapon* weapon);

	// 衝突時処理類
    void ResolveStageCollision(ColliderData& mover, const ColliderData& stage);
    void ResolveCapsuleCollision(ColliderData& a, ColliderData& b);
    void HitAttackToDamage(const ColliderData& self, const ColliderData& target, Player* player);

    // ヒット回数
    int hitCount_ = 0;
    std::unordered_set<int> hitEnemyIds_; // ヒット済み敵ID
};