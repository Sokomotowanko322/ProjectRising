#include <EffekseerForDXLib.h>  
#include "../Manager/ResourceManager.h"  
#include "../Manager/SceneManager.h"  
#include "../Utility/Utility.h"  
#include "../Common/Quaternion.h"  
#include "../Application.h"  
#include "Weapon.h"  

const VECTOR TITLESCENE_POS = { 1000.0f, 600.0f, 0.0f };
const VECTOR WEAPON_TITLE_SCALE = { 1.0f,1.0f,1.0f };
const VECTOR WEAPON_GAME_SCALE = { 0.05f,0.05f,0.05f };

Weapon::Weapon(void) : ActorBase(),
effectPos_{ Utility::VECTOR_ZERO },
effectScale_{ 10.0f,20.0f ,10.0f },
resMng_(ResourceManager::GetInstance()),
handRPos_{ 13.0f, 3.0f, 3.5f },
swordLength_{ 0.0f, 180.0f, 0.0f },
handAttachFrame_{ 34 },
handLocalquarot_({
Quaternion::Euler(
	Utility::Deg2RadF(90.0f),
	Utility::Deg2RadF(0.0f),
	Utility::Deg2RadF(20.0f)) }),
titleSceneQuarot_({
Quaternion::Euler(
	Utility::Deg2RadF(0.0f),
	Utility::Deg2RadF(180.0f),
	Utility::Deg2RadF(-30.0)) }),
	isEffect_(false)
{

}

Weapon::~Weapon(void)
{
	// モデル解放
	MV1DeleteModel(weaponTransform_.modelId);
}

void Weapon::Init(void)  
{  
	// モデル情報の初期化(座標などは敢えて初期化しない)
	weaponTransform_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_WEAPON);

	// 剣の軌跡
	effectLineResId_ = resMng_.Load(ResourceManager::SRC::PLAYER_WEAPON_SWORDLINE).handleId_;

}

void Weapon::TitleUpdate(void)  
{  
	// モデルの各情報をタイトル用に設定
	weaponTransform_.pos = TITLESCENE_POS;
	weaponTransform_.quaRotLocal = titleSceneQuarot_;
	weaponTransform_.scl = WEAPON_TITLE_SCALE;
	
	// 回転角度を増加 (毎フレーム回転させる)
	static float rotationAngle = 0.0f;

	// 毎フレーム1度回転 (ラジアンに変換)
	rotationAngle += Utility::Deg2RadF(1.0f); 

	// y軸回転のクォータニオンを作成
	Quaternion yAxisRotation = Quaternion::AngleAxis(rotationAngle, { 1.0f, 0.0f, 0.0f });

	// 初期の斜めの回転を適用
	weaponTransform_.quaRotLocal = Quaternion::Mult(titleSceneQuarot_, yAxisRotation);

	// 更新
	weaponTransform_.Update();
}

void Weapon::Update()
{
}

void Weapon::GameUpdate(Transform follow)
{  
	// エフェクト再生中のみ同期
	if (isEffect_) 
	{
		SyncEffect();
	}

	// 回転
	MATRIX mat;

	// 武器の位置同期	
	mat = MV1GetFrameLocalWorldMatrix(follow.modelId, handAttachFrame_);
	weaponTransform_.localPos = handRPos_;
	weaponTransform_.quaRotLocal = handLocalquarot_;
	auto rot = MGetRotElem(mat);
	effectPos_ = MGetTranslateElem(mat);

	// 行列からクオータニオン
	Quaternion qua = Quaternion::GetRotation(rot);
	Quaternion followRot = qua;

	// 武器の位置更新
	VECTOR swordPos = followRot.PosAxis(weaponTransform_.localPos);
	swordTopPos_ = followRot.PosAxis(swordLength_);
	swordTopPos_ = VAdd(effectPos_, swordTopPos_);
	weaponTransform_.pos = VAdd(effectPos_, swordPos);

	// 回転
	weaponTransform_.quaRot = qua;

	// 位置情報を追従対象の座標と合わせる
	transform_.pos = weaponTransform_.pos;

	// 武器の大きさを更新
	weaponTransform_.scl = WEAPON_GAME_SCALE;
	
	// すべての情報を更新
	transform_.Update();
	weaponTransform_.Update();
}  


void Weapon::Draw(void)
{ 
	// モデル描画
	MV1DrawModel(weaponTransform_.modelId);
}

void Weapon::SetPos(const VECTOR& pos)
{
}

const Transform& Weapon::GetWeaponTransform(void) const
{
	return weaponTransform_;
}

void Weapon::DrawDebug(void)
{  
}

VECTOR Weapon::GetPos(void) const
{
	return weaponTransform_.pos;
}

void Weapon::SyncEffect(void)
{
	weaponTransform_.Update();

	VECTOR FPos = weaponTransform_.pos;
	Quaternion FRot = weaponTransform_.quaRot;

#pragma region ローカル回転
	Quaternion rotL = Quaternion::Identity();

	rotL = rotL.Mult(handLocalquarot_);
	FRot = FRot.Mult(rotL);
#pragma endregion

	VECTOR angles = Quaternion::ToEuler(FRot);

	VECTOR localposL = { 30.0f,0.0f,30.0f };

	localposL = Quaternion::PosAxis(FRot, localposL);

	VECTOR posL = VAdd(FPos, localposL);

	SetPosPlayingEffekseer3DEffect(effectLinePlayId_, posL.x, posL.y, posL.z);
	SetRotationPlayingEffekseer3DEffect(effectLinePlayId_, angles.x, angles.y, angles.z);
	SetScalePlayingEffekseer3DEffect(effectLinePlayId_, effectScale_.x, effectScale_.y, effectScale_.z);
}

void Weapon::StartEffect(void)
{
	if (!isEffect_) 
	{
		effectLinePlayId_ = PlayEffekseer3DEffect(effectLineResId_);
		isEffect_ = true;
	}
}

void Weapon::StopEffect(void)
{
	if (isEffect_)
	{
		StopEffekseer3DEffect(effectLinePlayId_);
		isEffect_ = false;
	}
}