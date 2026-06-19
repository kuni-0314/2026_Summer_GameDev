#pragma once
#include "../../../Actor/ActorBase.h"
#include "../../../../Utility/AsoUtility.h"

class Player;

class ShotBase: public ActorBase
{
public:

    // 弾の共通初期化データ
    struct ShotData
    {
        float speed;
        float shotScl;
        int life;
        int power;
    };
    //弾種類
    enum class SHOT_TYPE
    {
        STRAIGHT,
    };

    //コンストラクタ
    ShotBase(const Transform& parentTransform, const ShotData& data, 
        int baseModelId, const VECTOR& dir, Player* player);
    //デストラクタ
    virtual ~ShotBase(void) override;
 
    //初期化・更新・描画・解放
    void Init(void);
    virtual void Update(void) override;
    virtual void Draw(void) override;
    virtual void Release(void) override;

    bool IsAlive(void) const { return isAlive_; }
    void SetAlive(bool alive) { isAlive_ = alive; }
    VECTOR GetPos(void) const { return transform_.pos; }

protected:

    //弾のパラメータ
    float speed_;
    float shotScl_;
    int life_;
    int power_;
    bool isAlive_;


    int shotModel_;

    //移動量
    VECTOR movePow_;
    // 移動方向
    VECTOR moveDir_;

    //移動スピード
    float moveSpeed_;

    // リソースロード
    virtual void InitLoad(void)  = 0;
    // 更新系
    virtual void UpdateProcess(void) = 0;
    virtual void UpdateProcessPost(void) = 0;



    Player* player_;
};



