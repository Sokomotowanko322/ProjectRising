#pragma once
#include <memory>
#include <vector>
#include "../Collider/ColliderData.h"
#include "../Object/Unit/ActorBase.h"

class ActorBase;

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

private:

    // 複数アクターを管理
    std::vector<std::weak_ptr<ActorBase>> actors_; 

	// コライダーのリスト
    std::vector<ColliderData> colliders_;
   
    // コライダの位置などを更新する
    void UpdateColliders();
	
    // 当たり判定をチェック
    void CheckCollisions();
    
	// 衝突時処理類
    void ResolveStageCollision(ColliderData& mover, const ColliderData& stage);
    void ResolveCapsuleCollision(ColliderData& a, ColliderData& b);
    void HitAttackToDamage(const ColliderData& self, const ColliderData& target);
};