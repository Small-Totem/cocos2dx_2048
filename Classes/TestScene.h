#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"

//注意:括号不要去掉(自己想想为什么)
#define ABS(x) ((x)>=0?(x):-(x))
#define _RGB(r,g,b) cocos2d::Color4F(cocos2d::Color4B(r,g,b,255))

class TestScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    
    //调用方法 : set_normal_node_touch_listener(sprite0, [](Touch* touch, Event* event) { //todo };
    //void set_normal_node_touch_listener(cocos2d::Node* node, std::function<void(cocos2d::Touch*, cocos2d::Event*)> call_back);
    
    //一次性的touch_listener,用于game_over时的点击事件
    void set_disposable_touch_listener(cocos2d::Node* node, std::function<void(cocos2d::Touch*, cocos2d::Event*)> call_back);
    
    //平常的move事件用的listener
    void set_global_touch_listener(cocos2d::Node* node,std::function<void(cocos2d::Touch*, cocos2d::Event*)> call_back);
   
    /*
    * 轮子,https://www.cnblogs.com/leehongee/p/3639623.html
    * 注意,此函数不完美,如果填充颜色alpha不为1可以发现bug
    *
    * @brief        画圆角矩形
    * @param        origin            矩形开始点
    * @param        destination        矩形结束点
    * @param        radius            圆角半径
    * @param        segments        圆角等份数，等份越多，圆角越平滑
    * @param        bFill            是否填充
    * @param        color            填充颜色
    */
    static void drawRoundRect(cocos2d::DrawNode*& draw_node, cocos2d::Point origin, cocos2d::Point destination, float radius, unsigned int segments, bool fill, cocos2d::Color4F color);

    void exec_move();

    //void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(TestScene);
};
#endif