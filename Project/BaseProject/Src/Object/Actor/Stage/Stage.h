#pragma once
#include "../ActorBase.h"
#include "../../../Renderer/ModelMaterial.h"
#include "../../../Renderer/ModelRenderer.h"
#include "../../../Manager/Camera.h"
#include <vector>
class Stage :public ActorBase
{
public:


	static constexpr float SCL_MAIN_STAGE_X = 3.0f;
	static constexpr float SCL_MAIN_STAGE_Y = 1.0f;
	static constexpr float SCL_MAIN_STAGE_Z = 3.0f;

	static constexpr VECTOR POS_MAIN_STAGE = { 0.0f, -100.0f, 0.0f };


	//コンストラクタ
	Stage();
	//デストラクタ
	~Stage() override;

	void Update() override;
	void Draw() override;



protected:

	// リソースロード
	void InitLoad() override;

	// 大きさ、回転、座標の初期化
	void InitTransform() override;

	// 衝突判定の初期化
	void InitCollider() override;

	// アニメーションの初期化
	void InitAnimation() override;

	// 初期化後の個別処理
	void InitPost() override;

private:

	//マテリアル
	std::unique_ptr<ModelMaterial> material_;
	//レンダー
	std::unique_ptr<ModelRenderer> renderer_;

	std::unique_ptr<Camera> camera_;

	Transform mainStage_;

	// 除外フレーム名称
	const std::vector<std::string> EXCLUDE_FRAME_NAMES = { "Mush", "Grass", };

	// 対象フレーム
	const std::vector<std::string> TARGET_FRAME_NAMES = { "Ground", };


};