#ifndef ___2048_H__
#define ___2048_H__

#include "cocos2d.h"

//���ڷ�if���� //������
//#define STR_EQU(str1,str2) (std::string(str1).compare(std::string(str2)) == 0) 

//������
#define CHEAT_ENABLED


class _2048
{
public:
	/* �����ʵ���ƶ���Ϊtrue
	�����ж��ֻ�����ȴû�п��ƶ�֮���
	���ֵ��ʱ��Ϊfalse,�� �ƶ���&&�����·���ǰ Ϊtrue */
	bool move_status = false;
	/*�����жϵȴ�����������������  ---> ��ϲ������callback����*/
	bool anim_end = true;
	cocos2d::Scene* scene;
	cocos2d::DrawNode* _2048_block[4][4] = {0};
	cocos2d::Label* _2048_block_label[4][4] = {0};
	float _2048_block_size;
	float _2048_origin_x;
	float _2048_origin_y;

	/*��Ӧ��ϵ
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

	/*���������,�����һ�㲻��,�Ӵζ��㿪ʼ���±���,���ж��Ƿ��ƶ�/�ϲ�*/
	void up();
	void down();
	void left();
	void right();

	/* @return �յĿ�֮����*/
	int get_spare_count();
	/* @param force �Ƿ�ǿ������*/
	void generate_block(bool use_animation=false,bool force=false);
	void normal_generate();

	void graphics_new_block(int x, int y);
	void graphics_delete_block(int x, int y, bool use_anim=false);
	void graphics_delete_block(cocos2d::Node* block, cocos2d::Node* label, bool use_anim = false);
	void graphics_move_block(int x, int y, int move_length, int orientation, bool merge=false);


	bool judge_game_over();
	void print_game_over();
	void game_over_onclick();

	/*  @brief ����Ļ��������תΪnum[][]�������
		@return ���괦�Ŀ��Ӧ��num[][]
		eg. Vec2(1,2)����Ӧnum[1][2]*/
	cocos2d::Vec2* get_num_from_position(int x, int y);



private:
	/*@return ��ȷ�����Ǹ�x��y����(���ʵ�ִ���ע��)*/
	int get_a_valid_block(int x,int y,int orientation);
	bool judge_game_not_over(int x, int y);
	static cocos2d::Color4F get_color(int num);
};

#endif