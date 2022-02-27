#ifndef ___2048_H__
#define ___2048_H__

#include "cocos2d.h"

//用于放if里面 //已弃用
//#define STR_EQU(str1,str2) (std::string(str1).compare(std::string(str2)) == 0) 

//作弊项
#define CHEAT_ENABLED


class _2048
{
public:
	/* 如果有实际移动则为true
	用于判断手滑动了却没有块移动之情况
	这个值长时间为false,仅 移动后&&生成新方块前 为true */
	bool move_status = false;
	/*用于判断等待动画结束才做的事  ---> 块合并情况的callback函数*/
	bool anim_end = true;
	cocos2d::Scene* scene;
	cocos2d::DrawNode* _2048_block[4][4] = {0};
	cocos2d::Label* _2048_block_label[4][4] = {0};
	float _2048_block_size;
	float _2048_origin_x;
	float _2048_origin_y;

	/*对应关系
	num[0][3] ...                 num[3][3]
	num[0][2] ...
	num[0][1] ...
	num[0][0] num[1][0] num[2][0] num[3][0]
	*/
	int num[4][4] = {0};

	_2048(cocos2d::Scene* _scene, float __2048_block_size,
		float __2048_origin_x, float __2048_origin_y) {
		scene = _scene;
		_2048_block_size = __2048_block_size;
		_2048_origin_x = __2048_origin_x;
		_2048_origin_y = __2048_origin_y;
	}

	void init();
	void print();

	/*如果是向上,则最顶上一层不管,从次顶层开始往下遍历,并判断是否移动/合并*/
	void up();
	void down();
	void left();
	void right();

	/* @return 空的块之数量*/
	int get_spare_count();
	/* @param force 是否强制生成*/
	void generate_block(bool use_animation=false,bool force=false);
	void normal_generate();

	void graphics_new_block(int x, int y);
	void graphics_delete_block(int x, int y, bool use_anim=false);
	void graphics_delete_block(cocos2d::Node* block, cocos2d::Node* label, bool use_anim = false);
	void graphics_move_block(int x, int y, int move_length, int orientation, bool merge=false);


	bool judge_game_over();
	void print_game_over();
	void game_over_onclick();

	/*  @brief 把屏幕绝对坐标转为num[][]相对坐标
		@return 坐标处的块对应的num[][]
		eg. Vec2(1,2)即对应num[1][2]*/
	cocos2d::Vec2* get_num_from_position(int x, int y);



private:
	/*@return 不确定的那个x或y坐标(详见实现处的注释)*/
	int get_a_valid_block(int x,int y,int orientation);
	bool judge_game_not_over(int x, int y);
	cocos2d::Color4F get_color(int num);
};

#endif