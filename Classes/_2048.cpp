#include "_2048.h"
#include "TestScene.h"

#define _RGB(r,g,b) cocos2d::Color4F(cocos2d::Color4B(r,g,b,255))
constexpr auto UP = 0;
constexpr auto DOWN = 1;
constexpr auto LEFT = 2;
constexpr auto RIGHT = 3;

//���CC_CALLBACK��η�,���ø���,��ȫ�ֱ�����
cocos2d::DrawNode* game_over;
cocos2d::Label* game_over_label;

void _2048::init() {
	generate_block(false, true);
	generate_block(false, true);

	if (rand() % 2 == 0) {//50%��������һ��
		generate_block(false, true);
	}
}

void _2048::print() {
	printf("------------------\n");
	for (int i = 3;i >= 0;i--) {
		for (int j = 0;j < 4;j++) {
			if(num[j][i]==0)
				printf("- ");
			else
				printf("%d ",num[j][i]);
		}
		printf("\n");
	}
	printf("------------------\n");
}

int _2048::get_spare_count() {
	int count = 0;
	for (int i = 0;i < 4;i++) {
		for (int j = 0;j < 4;j++) {
			if (num[i][j] == 0)
				count++;
		}
	}
	return count;
}

void _2048::generate_block(bool use_animation,bool force) {//forceĬ��ֵΪfalse(������.h)  forceָû�п��ƶ�ʱҲ����
	if (get_spare_count() == 0) {
		printf("err@generate_block:game_over\n");
		return;
	}

	if (!force && !move_status) {
		return;
	}
	
	//�����õ�ʵ��,����
	while (1) {
		int x = rand()%4, y = rand()%4;
		if (num[x][y] == 0) {
			//���������ֵĸ���: 2 50% 4 40% 8 10%
			int random_num = rand()%10;
			if(random_num < 5)
				num[x][y] = 2;
			else if(random_num < 9)
				num[x][y] = 4;
			else
				num[x][y] = 8;

			printf("->%d-(%d,%d)\n",num[x][y],x,y);
			graphics_new_block(x, y);
			break;
		}
	}
	move_status = false;

	
	if (get_spare_count() == 0) {
		if (judge_game_over()) {
			print_game_over();
			printf("\ngame_over!!!\n\n");
		}
	}
}

void _2048::normal_generate() {
	generate_block();
	if (rand() % 10 == 0 && get_spare_count() != 0) {//10%��������һ��
		generate_block();
	}
	print();
}

int _2048::get_a_valid_block(int x,int y,int orientation) {
	//����:get_a_valid_block(0,0,up) ��˼��Ѱ��[0][0]�Ϸ��ĵ�һ����0���λ��
	//(��ʱ��ȷ�����Ǹ��������Ǹ���0���yֵ)
	/*����ֵΪ ��ȷ�����Ǹ�x��y����*/
	if ((orientation == UP && y >= 3)||
		(orientation == DOWN && y <= 0)||
		(orientation == LEFT && x <= 0)||
		(orientation == RIGHT && x >= 3)) {
		//�жϲ��ý�������������
		printf("err@_2048::get_a_valid_block\n");
		return -2;
	}
	if (orientation == UP) {
		for (int j = y + 1;j < 4;j++) {
			if (num[x][j] != 0)
				return j;
		}
	}
	else if (orientation == DOWN) {
		for (int j = y - 1;j >= 0;j--) {
			if (num[x][j] != 0)
				return j;
		}
	}
	else if (orientation == LEFT) {
		for (int i = x - 1;i >= 0;i--) {
			if (num[i][y] != 0)
				return i;
		}
	}
	else if (orientation == RIGHT) {
		for (int i = x + 1;i < 4;i++) {
			if (num[i][y] != 0)
				return i;
		}
	}
	return -1;//û���ҵ���Чֵ
}

cocos2d::Color4F _2048::get_color(int num) {
	switch (num) {
	case 2:
		return _RGB(100, 110, 130);
	case 4:
		return _RGB(139, 156, 183);
	case 8:
		return _RGB(100, 200, 0);
	case 16:
		return _RGB(50, 144, 255);
	case 32:
		return _RGB(0, 191, 255);
	case 64:
		return _RGB(131, 122, 255);
	case 128:
		return _RGB(205, 190, 112);
	case 256:
		return _RGB(255, 215, 0);
	case 512:
		return _RGB(220, 80, 20);
	case 1024:
		return _RGB(255, 106, 106);
	case 2048:
		return _RGB(255, 193, 193);
	default:
		return _RGB(255, 0, 0);
	}
}

bool _2048::judge_game_over() {
	//�ж� get_spare_count() == 0 ��ſɵ��ô˺���
	return !judge_game_not_over(0, 0);
}
bool _2048::judge_game_not_over(int x,int y) {
	//�ݹ�������п�(�����µ�����) Ѱ����û���������ŵ�һ����
	//��������ظ��ıȽϰ�,�����Ż���
	if (x >= 4 || y >= 4)
		return false;
	//����vs2019������,������ʵû������
	if (x + 1 <= 3)
		if(num[x][y] == num[x + 1][y])
			return true;
	if (y + 1 <= 3)
		if(num[x][y] == num[x][y + 1])
			return true;
	if (judge_game_not_over(x + 1, y))
		return true;
	if (judge_game_not_over(x, y + 1))
		return true;
	return false;
}
void _2048::print_game_over() {
	game_over = cocos2d::DrawNode::create();
	game_over->setContentSize(cocos2d::Size(_2048_block_size*6, _2048_block_size*2));
	game_over->setPosition(_2048_origin_x + (20 + _2048_block_size) * 2 + 10,
		_2048_origin_y + (20 + _2048_block_size) * 2 + 10);

	game_over_label = cocos2d::Label::createWithTTF("game over!", "fonts/fzmw.ttf", 160);
	game_over_label->setPosition(_2048_origin_x + (20 + _2048_block_size) * 2 + 10,
		_2048_origin_y + (20 + _2048_block_size) * 2 + 10);

	TestScene::drawRoundRect(game_over, cocos2d::Vec2(-_2048_block_size * 3 + 40 , -_2048_block_size + 40), cocos2d::Vec2(_2048_block_size * 3 - 40, _2048_block_size - 40),
		24, 100, true, get_color(2048));

	game_over->setScale(0.2);
	game_over_label->setScale(0.2);

	scene->addChild(game_over, 3);
	scene->addChild(game_over_label, 3);
	auto scale_animation = cocos2d::EaseIn::create(
		cocos2d::ScaleTo::create(0.6, 1.0f), 0.3);

	game_over->runAction(scale_animation->clone());
	game_over_label->runAction(scale_animation);

	((TestScene*)scene)->set_disposable_touch_listener(game_over, CC_CALLBACK_0(_2048::game_over_onclick,this));
}
void _2048::game_over_onclick() {
	graphics_delete_block(game_over, game_over_label, true);
	for (int i = 0;i < 4; i++) {
		for (int j = 0;j < 4; j++) {
			num[i][j] = 0;
			graphics_delete_block(_2048_block_label[i][j], _2048_block[i][j], true);
		}
	}
	init();
}


void _2048::up() {
	for (int i = 0;i < 4;i++) {
		for (int j = 2;j >=0;j--) {
			if (num[i][j] == 0)//��ǰ��Ϊ��
				continue;
			int valid_position = get_a_valid_block(i, j, UP);
			if (valid_position == -1) {//�ƶ�����λ
				num[i][3] = num[i][j];
				num[i][j] = 0;
				move_status = true;
				graphics_move_block(i, j, 3 - j, UP);
				continue;
			}
			else {
				if (num[i][valid_position] == num[i][j]) {//�ϲ���
					num[i][valid_position] *= 2;
					num[i][j] = 0;
					move_status = true;
					graphics_move_block(i, j, valid_position - j, UP, true);
					continue;
				}
				else
				{
					if (num[i][valid_position-1] == 0) {//���ƶ���
						num[i][valid_position-1] = num[i][j];
						num[i][j] = 0;
						move_status = true;
						graphics_move_block(i, j, (valid_position - 1) - j, UP);
					}
					else//����
						continue;
				}
			}
		}
	}
}
void _2048::down() {
	for (int i = 0;i < 4;i++) {
		for (int j = 1;j <= 3;j++) {
			if (num[i][j] == 0)//��ǰ��Ϊ��
				continue;
			int valid_position = get_a_valid_block(i, j, DOWN);
			if (valid_position == -1) {//�ƶ�����λ
				num[i][0] = num[i][j];
				num[i][j] = 0;
				move_status = true;
				graphics_move_block(i, j, j, DOWN);
				continue;
			}
			else {
				if (num[i][valid_position] == num[i][j]) {//�ϲ���
					num[i][valid_position] *= 2;
					num[i][j] = 0;
					move_status = true;
					graphics_move_block(i, j, j - valid_position, DOWN, true);
					continue;
				}
				else
				{
					if (num[i][valid_position + 1] == 0) {//���ƶ���
						num[i][valid_position + 1] = num[i][j];
						num[i][j] = 0;
						move_status = true;
						graphics_move_block(i, j,j-(valid_position + 1), DOWN);
					}
					else//����
						continue;
				}
			}
		}
	}
}
void _2048::left() {
	for (int i = 1;i <= 3;i++) {
		for (int j = 0;j < 4;j++) {
			if (num[i][j] == 0)//��ǰ��Ϊ��
				continue;
			int valid_position = get_a_valid_block(i, j, LEFT);
			if (valid_position == -1) {//�ƶ�����λ
				num[0][j] = num[i][j];
				num[i][j] = 0;
				move_status = true;
				graphics_move_block(i, j, i, LEFT);
				continue;
			}
			else {
				if (num[valid_position][j] == num[i][j]) {//�ϲ���
					num[valid_position][j] *= 2;
					num[i][j] = 0;
					move_status = true;
					graphics_move_block(i, j, i - valid_position, LEFT, true);
					continue;
				}
				else
				{
					if (num[valid_position + 1][j] == 0) {//���ƶ���
						num[valid_position + 1][j] = num[i][j];
						num[i][j] = 0;
						move_status = true;
						graphics_move_block(i, j, i - (valid_position + 1), LEFT);
					}
					else//����
						continue;
				}
			}
		}
	}
}
void _2048::right() {
	for (int i = 2;i >= 0;i--) {
		for (int j = 0;j < 4 ;j++) {
			if (num[i][j] == 0)//��ǰ��Ϊ��
				continue;
			int valid_position = get_a_valid_block(i, j, RIGHT);
			if (valid_position == -1) {//�ƶ�����λ
				num[3][j] = num[i][j];
				num[i][j] = 0;
				move_status = true;
				graphics_move_block(i, j, 3 - i, RIGHT);
				continue;
			}
			else {
				if (num[valid_position][j] == num[i][j]) {//�ϲ���
					num[valid_position][j] *= 2;
					num[i][j] = 0;
					move_status = true;
					graphics_move_block(i, j, valid_position- i, RIGHT, true);
					continue;
				}
				else
				{
					if (num[valid_position - 1][j] == 0) {//���ƶ���
						num[valid_position - 1][j] = num[i][j];
						num[i][j] = 0;
						move_status = true;
						graphics_move_block(i, j, (valid_position - 1) - i, RIGHT);
					}
					else//����
						continue;
				}
			}
		}
	}
}


void _2048::graphics_new_block(int x,int y) {
	if (x > 3 || y > 3 || x < 0 || y < 0) {
		printf("err@graphics_new_block\n");
		return;
	}

	/* ���ù�,��Ϊcocos2d::Node���Զ��ͷ�,�����ڴ�й©
	if (_2048_block[x][y] != 0) {
		printf("warning@graphics_new_block:override_block\n");
	}*/

	_2048_block[x][y] = cocos2d::DrawNode::create();
	_2048_block[x][y]->setContentSize(cocos2d::Size(_2048_block_size, _2048_block_size));
	_2048_block[x][y]->setPosition(_2048_origin_x + (20 + _2048_block_size)* (x + 1) - _2048_block_size / 2,
		_2048_origin_y + (20 + _2048_block_size) * (y + 1) - _2048_block_size / 2);


	_2048_block_label[x][y] = cocos2d::Label::createWithTTF(std::to_string(num[x][y]),"fonts/fzmw.ttf",
		num[x][y] < 10 ? 120 : (num[x][y] < 1000 ? 100 : (num[x][y] < 10000 ? 80 : 60)));//4�������С:120,100,,80,60

	_2048_block_label[x][y]->setPosition(_2048_origin_x + (20 + _2048_block_size) * (x + 1) - _2048_block_size / 2,
		_2048_origin_y + (20 + _2048_block_size) * (y + 1) - _2048_block_size / 2);

	_2048_block[x][y]->setScale(0.2);
	_2048_block_label[x][y]->setScale(0.2);

	TestScene::drawRoundRect(_2048_block[x][y], cocos2d::Vec2(-_2048_block_size / 2, -_2048_block_size / 2), cocos2d::Vec2(_2048_block_size / 2, _2048_block_size / 2),
		24, 100, true, get_color(num[x][y]));

	scene->addChild(_2048_block[x][y], 2);
	scene->addChild(_2048_block_label[x][y], 2);
	auto scale_animation = cocos2d::EaseIn::create(
		cocos2d::ScaleTo::create(0.4, 1.0f), 0.3);

	_2048_block[x][y]->runAction(scale_animation->clone());
	_2048_block_label[x][y]->runAction(scale_animation);
}
void _2048::graphics_delete_block(int x, int y, bool use_anim) {
	if (x > 3 || y > 3 || x < 0 || y < 0) {
		printf("err@graphics_delete_block:input_err\n");
		return;
	}
	if (_2048_block[x][y] == 0 || _2048_block_label[x][y] == 0) {
		printf("warning@graphics_delete_block:nullptr,x=%d,y=%d\n",x,y);
	}
	if (_2048_block[x][y] != 0) {
		if (use_anim) {
			auto anim = cocos2d::Sequence::create(cocos2d::FadeOut::create(0.6f),
				cocos2d::CallFunc::create([=]() {
					scene->removeChild(_2048_block[x][y], false);
					_2048_block[x][y] = 0;
					anim_end = true;
					}
			), nullptr);
			_2048_block[x][y]->runAction(anim);
		}
		else {
			scene->removeChild(_2048_block[x][y], false);
			_2048_block[x][y] = 0;
		}
	}
	if (_2048_block_label[x][y] != 0) {
		if (use_anim) {
			auto anim = cocos2d::Sequence::create(cocos2d::FadeOut::create(0.6f),
				cocos2d::CallFunc::create([=]() {
					scene->removeChild(_2048_block_label[x][y], false);
					_2048_block_label[x][y] = 0;
					anim_end = true;
					}
			), nullptr);
			_2048_block_label[x][y]->runAction(anim);
		}
		else {
			scene->removeChild(_2048_block_label[x][y], false);
			_2048_block_label[x][y] = 0;
		}
	}
	anim_end = false;
	/*ֻ�кϲ����õ����������,���Բ��ð����ֹ���*/
	//num[x][y] = 0;
}
void _2048::graphics_delete_block(cocos2d::Node* block, cocos2d::Node* label, bool use_anim) {
	//���ﻹͦ����
	//�������������������Ϊ �ϲ��������,��Ϊ'�ƶ���'��'������'(�ϲ���Ŀ�ĵ�)
	//����ƶ����ƶ������б�Ŀ��ƶ�������,�ͻ�����ɾ����Ŀ��ָ��.
	//�������������ֱ�ӽ����ƶ����ָ��,���ڻص�������ɾ���ƶ���.
	if (block == 0 || label == 0) {
		printf("warning@graphics_delete_block:nullptr");
	}
	if (block != 0) {
		if (use_anim) {
			auto anim = cocos2d::Sequence::create(cocos2d::FadeOut::create(0.6f),
				cocos2d::CallFunc::create([=]() {
					scene->removeChild(block, false);
					anim_end = true;
					}
			), nullptr);
			block->runAction(anim);
		}
		else
			scene->removeChild(block);
	}
	if (label != 0) {
		if (use_anim) {
			auto anim = cocos2d::Sequence::create(cocos2d::FadeOut::create(0.6f),
				cocos2d::CallFunc::create([=]() {
					scene->removeChild(label, false);
					anim_end = true;
					}
			), nullptr);
			label->runAction(anim);
		}
		else
			scene->removeChild(label);
	}
	anim_end = false;
	/*ֻ�кϲ����õ����������,���Բ��ð����ֹ���*/
	//num[x][y] = 0;
}
void _2048::graphics_move_block(int x, int y, int move_length, int orientation, bool merge) {
	cocos2d::EaseIn* animation = nullptr;
	cocos2d::Sequence* animation_with_callback = nullptr;
	constexpr auto duration = 0.3;
	constexpr auto rate = 0.3;
	cocos2d::Node* curr_block = _2048_block[x][y];
	cocos2d::Node* curr_label = _2048_block_label[x][y];
	if (orientation == UP) {
		animation = cocos2d::EaseIn::create(cocos2d::MoveBy::create(
			duration, cocos2d::Vec2(0, (_2048_block_size + 20) * move_length)), rate);
		if (merge) {
			animation_with_callback = cocos2d::Sequence::create(animation,
				cocos2d::CallFunc::create([=]() {
					//printf("x=%d,y=%d,num[x][y]=%d,move_length=%d\n", x, y, num[x][y], move_length);
					graphics_delete_block(curr_block, curr_label);
					graphics_delete_block(x, y + move_length);
					graphics_new_block(x, y + move_length);
					anim_end = true;
					normal_generate();
					}
			), nullptr);
		}
		else {
			_2048_block[x][y + move_length] = _2048_block[x][y];
			_2048_block_label[x][y + move_length] = _2048_block_label[x][y];
		}
	}
	else if (orientation == DOWN) {
		animation = cocos2d::EaseIn::create(cocos2d::MoveBy::create(
			duration, cocos2d::Vec2(0, -(_2048_block_size + 20) * move_length)), rate);
		if (merge) {
			animation_with_callback = cocos2d::Sequence::create(animation,
				cocos2d::CallFunc::create([=]() {
					graphics_delete_block(curr_block, curr_label);
					graphics_delete_block(x, y - move_length);
					graphics_new_block(x, y - move_length);
					anim_end = true;
					normal_generate();
					}
			), nullptr);
		}
		else {
			_2048_block[x][y - move_length] = _2048_block[x][y];
			_2048_block_label[x][y - move_length] = _2048_block_label[x][y];
		}
	}
	else if (orientation == LEFT) {
		animation = cocos2d::EaseIn::create(cocos2d::MoveBy::create(
			duration, cocos2d::Vec2(-(_2048_block_size + 20) * move_length,0)), rate);
		if (merge) {
			animation_with_callback = cocos2d::Sequence::create(animation,
				cocos2d::CallFunc::create([=]() {
					graphics_delete_block(curr_block, curr_label);
					graphics_delete_block(x - move_length, y);
					graphics_new_block(x - move_length, y);
					anim_end = true;
					normal_generate();
					}
			), nullptr);
		}
		else {
			_2048_block[x - move_length][y] = _2048_block[x][y];
			_2048_block_label[x - move_length][y] = _2048_block_label[x][y];
		}
	}
	else if (orientation == RIGHT) {
		animation = cocos2d::EaseIn::create(cocos2d::MoveBy::create(
			duration, cocos2d::Vec2((_2048_block_size + 20) * move_length, 0)), rate);
		if (merge) {
			animation_with_callback = cocos2d::Sequence::create(animation,
				cocos2d::CallFunc::create([=]() {
					graphics_delete_block(curr_block, curr_label);
					graphics_delete_block(x + move_length, y);
					graphics_new_block(x + move_length, y);
					anim_end = true;
					normal_generate();
					}
			), nullptr);
		}
		else {
			_2048_block[x + move_length][y] = _2048_block[x][y];
			_2048_block_label[x + move_length][y] = _2048_block_label[x][y];
		}
	}
	if (animation == nullptr||(animation_with_callback == nullptr && merge)) {
		printf("err@graphics_move_block:nullptr");
		return;
	}

	if (merge) {//ֻҪһ��callback
		_2048_block[x][y]->runAction(animation_with_callback);
		_2048_block_label[x][y]->runAction(animation->clone());
		anim_end = false;
	}
	else {
		_2048_block[x][y]->runAction(animation->clone());
		_2048_block_label[x][y]->runAction(animation);
		_2048_block[x][y] = 0;
		_2048_block_label[x][y] = 0;
	}
}


#ifdef CHEAT_ENABLED

#endif
