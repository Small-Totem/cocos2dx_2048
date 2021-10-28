//output -> \..\bin\test0\Debug\test0.exe
//vs2019:ctrl+m+o 折叠所有,ctrl+m+l 展开所有
#include "TestScene.h"
#include "_2048.h"

//注意:括号不要去掉(自己想想为什么)
#define ABS(x) ((x)>=0?(x):-(x))
#define _RGB(r,g,b) Color4F(Color4B(r,g,b,255))

USING_NS_CC;

Point touchBegan = Vec2(0, 0);
Point touchEnded = Vec2(0, 0);
Size visibleSize;
Vec2 origin;

_2048* _2048_;

Scene* TestScene::createScene()
{
    return TestScene::create();
}

void TestScene::exec_move() {
    int move_x = touchBegan.x - touchEnded.x;
    int move_y = touchBegan.y - touchEnded.y;

    if (ABS(move_x) < 10 && ABS(move_y) < 10) {
        _2048_->print();
        return;
    }
    if (!_2048_->anim_end) {
        printf("!anim_end. move canceled.\n");
        return;
    }

    if (ABS(move_x) > ABS(move_y)) {
        if (move_x > 0) {
            _2048_->left();
            printf("←\n");
        }
        else {
            _2048_->right();
            printf("→\n");
        }
    }
    else{
        if (move_y > 0) {
            _2048_->down();
            printf("↓\n");
        }
        else {
            _2048_->up();
            printf("↑\n");
        }
    }

    if (_2048_->move_status && _2048_->anim_end) {
        _2048_->normal_generate();
    }
}

/*
void TestScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
*/


//此函数为之前测试listener的时候搞的,未使用
/*
void TestScene::set_normal_node_touch_listener(cocos2d::Node* node, 
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> call_back) {
    //cocos2dx的点击事件是全局的 需要自己判断点到哪了
    //注意,这个函数整的并不好,调用两次或以上可能出问题(onTouchxx里未经判断的东西被调用多次)
    // update: 点击对象(可能应该?)使用回调,自行搜索CC_CALLBACK_0
    auto touch_listener0 = EventListenerTouchOneByOne::create();
    touch_listener0->onTouchBegan = [=](Touch* touch, Event* event) {
        //true则不会向下层传递
        touchBegan = Vec2(touch->getLocation().x, touch->getLocation().y);
        if (!(ABS(touch->getLocation().x - node->getPosition().x) > node->getContentSize().width / 2 ||
            ABS(touch->getLocation().y - node->getPosition().y) > node->getContentSize().height / 2))
            node->setName("TouchBegan");
        else node->setName("null");
        return true;
    };
    touch_listener0->onTouchEnded = [=](Touch* touch, Event* event) {
        //注意 此处的捕获列表不能用引用传值
        touchEnded = Vec2(touch->getLocation().x, touch->getLocation().y);

        printf("%f,%f    %f,%f\n",touch->getLocation().x, touch->getLocation().y, node->getPosition().x, node->getPosition().y);
        printf("%f\n\n", ABS(touch->getLocation().x - node->getPosition().x));
        if (ABS(touch->getLocation().x - node->getPosition().x) > node->getContentSize().width / 2 ||
            ABS(touch->getLocation().y - node->getPosition().y) > node->getContentSize().height / 2)
            return;
        if(std::string("TouchBegan").compare(std::string(node->getName()))!=0)//node->getName()不为"TouchBegan"时
            return;

        call_back(touch, event);
        node->setName("null");
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener0, node);
}*/

void TestScene::set_disposable_touch_listener(cocos2d::Node* node, 
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> call_back){
    auto touch_listener0 = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener0->onTouchBegan = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
        return true;
    };
    touch_listener0->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
        call_back(touch, event);
        _eventDispatcher->removeEventListener(touch_listener0);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener0, node);
}

void TestScene::set_global_touch_listener(cocos2d::Node* node,
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> call_back) {
    auto touch_listener0 = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener0->onTouchBegan = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
        touchBegan = Vec2(touch->getLocation().x, touch->getLocation().y);
        return true;
    };
    touch_listener0->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
        touchEnded = Vec2(touch->getLocation().x, touch->getLocation().y);
        printf("%f,%f    %f,%f\n", touch->getLocation().x, touch->getLocation().y, node->getPosition().x, node->getPosition().y);
        printf("%f\n\n", ABS(touch->getLocation().x - node->getPosition().x));
        call_back(touch, event);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener0, node);
}

void TestScene::drawRoundRect(DrawNode*& draw_node, Point origin, Point destination,
    float radius, unsigned int segments, bool fill, Color4F color)
{
    //算出1/4圆
    const float coef = 0.5f * (float)M_PI / segments;
    Point* vertices = new Point[segments + 1];
    Point* thisVertices = vertices;
    for (unsigned int i = 0; i <= segments; ++i, ++thisVertices)
    {
        float rads = (segments - i) * coef;
        thisVertices->x = (int)(radius * sinf(rads));
        thisVertices->y = (int)(radius * cosf(rads));
    }
    //
    Point tagCenter;
    float minX = MIN(origin.x, destination.x);
    float maxX = MAX(origin.x, destination.x);
    float minY = MIN(origin.y, destination.y);
    float maxY = MAX(origin.y, destination.y);

    unsigned int dwPolygonPtMax = (segments + 1) * 4;
    Point* pPolygonPtArr = new Point[dwPolygonPtMax];
    Point* thisPolygonPt = pPolygonPtArr;
    int aa = 0;
    //左上角
    tagCenter.x = minX + radius;
    tagCenter.y = maxY - radius;
    thisVertices = vertices;
    for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
    {
        thisPolygonPt->x = tagCenter.x - thisVertices->x;
        thisPolygonPt->y = tagCenter.y + thisVertices->y;
        ++aa;
    }
    //右上角
    tagCenter.x = maxX - radius;
    tagCenter.y = maxY - radius;
    thisVertices = vertices + segments;
    for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
    {
        thisPolygonPt->x = tagCenter.x + thisVertices->x;
        thisPolygonPt->y = tagCenter.y + thisVertices->y;
        ++aa;
    }
    //右下角
    tagCenter.x = maxX - radius;
    tagCenter.y = minY + radius;
    thisVertices = vertices;
    for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
    {
        thisPolygonPt->x = tagCenter.x + thisVertices->x;
        thisPolygonPt->y = tagCenter.y - thisVertices->y;
        ++aa;
    }
    //左下角
    tagCenter.x = minX + radius;
    tagCenter.y = minY + radius;
    thisVertices = vertices + segments;
    for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
    {
        thisPolygonPt->x = tagCenter.x - thisVertices->x;
        thisPolygonPt->y = tagCenter.y - thisVertices->y;
        ++aa;
    }

    if (fill) {
        draw_node->drawSolidPoly(pPolygonPtArr, dwPolygonPtMax, color);
    }
    else
    {
        draw_node->drawPoly(pPolygonPtArr, dwPolygonPtMax, true, color);
    }

    CC_SAFE_DELETE_ARRAY(vertices);
    CC_SAFE_DELETE_ARRAY(pPolygonPtArr);
}

bool TestScene::init()
{
    if (!Scene::init())
        return false;

    srand(time(0));

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    auto background = DrawNode::create();
    background->drawSolidRect(Vec2(origin.x,origin.y),Vec2(origin.x + visibleSize.width,
        origin.y + visibleSize.height), _RGB(36, 36, 36));
    this->addChild(background, 0);

    //退出按钮
    /*
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(TestScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + visibleSize.height - closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    */


    //左上角label
    std::string c_time = std::string("compile_time=")+std::string(__DATE__) 
        + std::string(",") + std::string(__TIME__) + std::string("; ")
        + std::string("target_platform=") + std::string(
            CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 ? "WIN32" :
            (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ? "ANDROID" :
                (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX ? "LINUX" : std::to_string(CC_TARGET_PLATFORM))))
        + std::string("\nhttps://github.com/Small-Totem");
    auto label_compile_time = Label::createWithTTF(c_time, "fonts/fzmw.ttf", 30);
    if (label_compile_time == nullptr)
    {
       printf("failed to load 'fonts/fzmw.ttf'");
    }
    else
    {
        label_compile_time->setPosition(Vec2(origin.x + label_compile_time->getContentSize().width / 2,
            origin.y + visibleSize.height - label_compile_time->getContentSize().height/2));
 
        this->addChild(label_compile_time, 1);
    }
    

    //开始画2048之界面(背景)
    int _2048_background_size = visibleSize.height - 20 * 2;//上下留白20px

    float _2048_origin_x = origin.x + visibleSize.width / 2 - _2048_background_size / 2;
    float _2048_origin_y = origin.y + 20;
    //自动适应_2048_background的位置
    drawRoundRect(background, Vec2(_2048_origin_x, _2048_origin_y),
        Vec2(origin.x + visibleSize.width / 2 + _2048_background_size / 2, origin.y + visibleSize.height - 20),
        50, 100, true, _RGB(56, 56, 56));

    float _2048_block_size = (_2048_background_size - 20 * 5) / 4;//4x4的块之间留白20px
    for (int i = 1; i < 5; i++) {
        for (int j = 1;j < 5; j++) {
            //自动适应
            drawRoundRect(background,
                Vec2(_2048_origin_x + 20 + (20 + _2048_block_size) * (i-1), _2048_origin_y+20 + (20 + _2048_block_size) * (j - 1)),
                Vec2(_2048_origin_x + (20 + _2048_block_size)*i, _2048_origin_y + (20 + _2048_block_size)*j),
                24, 100, true, _RGB(112, 104, 77));
        }
    }

    set_global_touch_listener(background,
        [=](Touch* touch, Event* event) {
            exec_move();
        });

    _2048_ = new _2048(this, _2048_block_size, _2048_origin_x, _2048_origin_y);



#ifdef _2048_test
   int curr_num = 1;
    for (int i = 0;i < 4; i++) {
        for (int j = 0;j < 4; j++) {
            _2048_->num[i][j] = curr_num>2?curr_num:2;
            _2048_->graphics_new_block(i, j);
            curr_num *= 2;
        }
    }
#else
    _2048_->init();
#endif


    return true;
}
