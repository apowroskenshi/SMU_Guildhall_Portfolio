#include<allegro.h>
struct snake_body
{
	int x, y, pos, prev_x, prev_y, prev_pos;
	snake_body* prev, * next;
};

snake_body* head, * tail;

BITMAP* head_img[4], * tail_img[4], * body_img[4];

BITMAP* buffer;

BITMAP* background, * food;
FONT* f, * f1, * f2;

int flag = 1;
int ext = 0;
int score = 1;
int food_x, food_y;
int food_alive = 0, food_flag = 1;
int rev_flag = 0;
int normal_snake = 0;
int decreasing_snake = 0;
int increasing_snake = 0;
int decrease_count = 50;
int increase_count = 20;
int difficulty = 1;
#define RED makecol(255,0,0)
#define WHITE makecol(255,255,255)
#define BLACK makecol(0,0,0)
#define GREY makecol(50,50,50)

int inside(int x, int y, int left, int top, int right, int bottom)
{
	if (x > left && x < right && y > top && y < bottom)
		return 1;
	else
		return 0;
}
void initialize()
{
	head = new snake_body;
	tail = new snake_body;
	head->x = 1280 / 2;
	head->y = 720 / 2;
	head->prev = NULL;
	head->pos = 3;

	tail->x = (1280 / 2) + 32;
	tail->y = 720 / 2;
	tail->prev = head;
	tail->next = NULL;
	tail->pos = 3;
	head->next = tail;


	allegro_init();
	install_keyboard();
	set_color_depth(16);
	set_gfx_mode(GFX_SAFE, 1280, 720, 0, 0);
	buffer = create_bitmap(SCREEN_W, SCREEN_H);
	clear_bitmap(buffer);
	background = load_bitmap("resources/background.bmp", NULL);

}

void create_body()
{
	snake_body* body = new snake_body;

	body->x = tail->x;
	body->y = tail->y;
	body->prev = tail->prev;
	body->prev->next = body;
	tail->prev = body;
	body->next = tail;
	switch (tail->pos)
	{
	case 0:

		tail->y += 32;
		body->pos = 0;
		break;
	case 1:

		tail->x -= 32;
		body->pos = 1;
		break;
	case 2:

		tail->y -= 32;
		body->pos = 2;
		break;
	case 3:

		tail->y += 32;
		body->pos = 3;
		break;
	}
}

void delete_body()
{
	snake_body* todel = tail;

	tail = tail->prev;
	tail->next = NULL;

	delete todel;
}


void display()
{
	snake_body* disp = head->next;

	f1 = load_font("resources/Tekton.pcx", NULL, NULL);

	blit(background, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);

	if (normal_snake)
	{
		if (rev_flag)
			textprintf_ex(buffer, f1, 50, 10, GREY, -1, "WARNING SNAKE INPUT REVERSED");
		else
			textprintf_ex(buffer, f1, 50, 10, RED, -1, "WARNING SNAKE INPUT REVERSED");
	}
	textprintf_ex(buffer, f1, 900, 10, BLACK, -1, "SCORE :%d", score);
	if (!food_flag)
		draw_sprite(buffer, food, food_x, food_y);

	draw_sprite(buffer, head_img[head->pos], head->x, head->y);

	while (disp->next != NULL)
	{
		draw_sprite(buffer, body_img[disp->pos], disp->x, disp->y);

		disp = disp->next;
		//rest(50);
	}
	draw_sprite(buffer, tail_img[tail->pos], tail->x, tail->y);

	blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
}

void get_input()
{

	head->prev_x = head->x;
	head->prev_y = head->y;
	head->prev_pos = head->pos;


	if (key[KEY_UP])
	{
		if (decreasing_snake)
			decrease_count--;
		if (increasing_snake)
			increase_count--;
		if (rev_flag)
			goto DOWN;
	UP:
		flag = 0;
		switch (head->pos)
		{

		case 1:
		case 3:
		case 0:
			head->y -= 32;
			if (head->y < 32)
				head->y = SCREEN_H - 64;
			head->pos = 0;
			break;
		case 2:
			head->y += 32;
			if (head->y + 32 > (SCREEN_H - 32))
				head->y = 32;
			break;
		}
	}


	else if (key[KEY_DOWN])
	{
		if (decreasing_snake)
			decrease_count--;
		if (increasing_snake)
			increase_count--;
		if (rev_flag)
			goto UP;
	DOWN:
		flag = 0;
		switch (head->pos)
		{
		case 1:
		case 3:
		case 2:
			head->y += 32;
			if (head->y + 32 > (SCREEN_H - 32))
				head->y = 32;
			head->pos = 2;
			break;
		case 0:
			head->y -= 32;
			if (head->y < 32)
				head->y = SCREEN_H - 64;
			break;
		}
	}

	else if (key[KEY_RIGHT])
	{
		if (decreasing_snake)
			decrease_count--;
		if (increasing_snake)
			increase_count--;
		if (rev_flag)
			goto LEFT;
	RIGHT:
		flag = 0;
		switch (head->pos)
		{

		case 0:
		case 2:
		case 1:
			head->x += 32;
			if (head->x + 32 > (SCREEN_W - 32))
				head->x = 32;
			head->pos = 1;
			break;
		case 3:
			head->x -= 32;
			if (head->x < 32)
				head->x = SCREEN_W - 64;
			break;
		}
	}

	else if (key[KEY_LEFT])
	{
		if (decreasing_snake)
			decrease_count--;
		if (increasing_snake)
			increase_count--;
		if (rev_flag)
			goto RIGHT;
	LEFT:
		flag = 0;
		switch (head->pos)
		{

		case 0:
		case 2:
		case 3:
			head->x -= 32;
			if (head->x < 32)
				head->x = SCREEN_W - 64;
			head->pos = 3;
			break;
		case 1:
			head->x += 32;
			if (head->x + 32 > (SCREEN_W - 32))
				head->x = 32;
			break;

		}
	}
}

void destry()
{
	while (head->next->next != tail)

		delete_body();

}
void move_snake()
{
	snake_body* trav = head->next;

	while (trav)
	{
		trav->prev_x = trav->x;
		trav->prev_y = trav->y;
		trav->prev_pos = trav->pos;

		trav->x = trav->prev->prev_x;
		trav->y = trav->prev->prev_y;
		trav->pos = trav->prev->prev_pos;

		trav = trav->next;
	}

	if (flag)
	{
		if (decreasing_snake)
			decrease_count--;
		if (increasing_snake)
			increase_count--;

		switch (head->pos)
		{
		case 0:
			head->y -= 32;
			if (head->y < 32)
				head->y = SCREEN_H - 64;
			break;
		case 1:
			head->x += 32;
			if (head->x + 32 > (SCREEN_W - 32))
				head->x = 32;
			break;
		case 2:
			head->y += 32;
			if (head->y + 32 > (SCREEN_H - 32))
				head->y = 32;
			break;
		case 3:
			head->x -= 32;
			if (head->x < 32)
				head->x = SCREEN_W - 64;
			break;
		}
	}
	else
		flag = 1;


}

void pause_game()
{
	BITMAP* bmp = load_bitmap("resources\\background1.bmp", NULL);
	f = load_font("resources\\Andy.pcx", NULL, NULL);
	f2 = load_font("resources\\Sybil.pcx", NULL, NULL);

	int pos = 1, n;
	ext = 0;
	while (1)
	{
		blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		textprintf_ex(buffer, f2, 480, 100, WHITE, -1, "The Snake");
		textprintf_ex(buffer, f, 180, 355, GREY, -1, "Le'mme Continue");
		textprintf_ex(buffer, f, 210, 385, GREY, -1, "I Can't Take this anymore Get me outta here");

		switch (pos)
		{
		case 1:
			textprintf_ex(buffer, f, 180, 355, WHITE, -1, "Le'mme Continue");
			break;
		case 2:
			textprintf_ex(buffer, f, 210, 385, WHITE, -1, "I Can't Take this anymore Get me outta here");
			break;
		}
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		rest(100);

		if (key[KEY_UP])
			pos = 1;

		if (key[KEY_DOWN])
			pos = 2;

		if (key[KEY_ENTER])
		{
			switch (pos)
			{
			case 1:
				return;

			case 2:
				ext = 1;
				return;
			}
		}



	}
}

void gameover()
{
	BITMAP* bmp = load_bitmap("resources/background1.bmp", NULL);
	f = load_font("resources/Andy1.pcx", NULL, NULL);
	f2 = load_font("resources/kristen.pcx", NULL, NULL);


	int pos = 2, n;

	while (1)
	{
		blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);

		textprintf_ex(buffer, f, 450, 200, WHITE, -1, "Arghh!!  Watch Your Tail");
		textprintf_ex(buffer, f2, 520, 300, GREY, -1, "I Want More");
		textprintf_ex(buffer, f2, 550, 340, GREY, -1, "I'm too Scared To Continue");
		switch (pos)
		{
		case 1:
			textprintf_ex(buffer, f2, 520, 300, WHITE, -1, "I Want More");
			break;
		case 2:
			textprintf_ex(buffer, f2, 550, 340, WHITE, -1, "I'm too Scared To Continue");

			break;
		}

		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		rest(100);

		if (key[KEY_UP])
			pos = 1;

		if (key[KEY_DOWN])
			pos = 2;

		if (key[KEY_ENTER])
		{
			switch (pos)
			{
			case 1:

				return;

			case 2:
				f2 = load_font("resources\\Sybil.pcx", NULL, NULL);
				while (!key[KEY_ESC])
				{
					blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
					textprintf_ex(buffer, f2, 480, 100, WHITE, -1, "The Snake");
					textprintf_ex(buffer, f2, 480, 270, WHITE, -1, "A Game By");
					textprintf_ex(buffer, f2, 520, 320, WHITE, -1, "Swaroop Kishen");
					blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
				}
				exit(0);
			}
		}



	}
}

void check_collision()
{
	snake_body* trav = head->next;

	if (decreasing_snake)
	{
		if (head->next == tail)
		{
			gameover();
			ext = 1;
		}

	}

	if (inside(food_x + 16, food_y + 16, head->x, head->y, head->x + 32, head->y + 32))
	{
		food_flag = 1;
		if (normal_snake || decreasing_snake)
			create_body();

		if (increasing_snake)
			delete_body();
		if (!normal_snake)
		{
			if (difficulty == 1)
				score += 2;
			else
				score += (5 * difficulty);
		}
		else
		{
			if (difficulty == 1)
				score += 1;
			else
				score += difficulty;
		}
	}
	else
		switch (head->pos)
		{

		case 0:
			if ((head->x + 16) > (food_x) && head->x + 16 < food_x + 32 && head->y > food_y && head->y < (food_y + 32))
			{
				food_flag = 1;
				if (normal_snake || decreasing_snake)
					create_body();

				if (increasing_snake)
					delete_body();
				if (!normal_snake)
				{
					if (difficulty == 1)
						score += 2;
					else
						score += (5 * difficulty);
				}
				else
				{
					if (difficulty == 1)
						score += 1;
					else
						score += difficulty;
				}
			}
			break;

		case 1:
			if ((head->x + 32) > (food_x) && head->x + 32 < food_x + 32 && head->y + 16 > food_y && head->y + 16 < (food_y + 32))
			{
				food_flag = 1;
				if (normal_snake || decreasing_snake)
					create_body();

				if (increasing_snake)
					delete_body();
				if (!normal_snake)
				{
					if (difficulty == 1)
						score += 2;
					else
						score += (5 * difficulty);
				}
				else
				{
					if (difficulty == 1)
						score += 1;
					else
						score += difficulty;
				}
			}
			break;

		case 2:
			if ((head->x + 16) > (food_x) && head->x + 16 < food_x + 32 && head->y + 32 > food_y && head->y + 32 < (food_y + 32))
			{
				food_flag = 1;
				if (normal_snake || decreasing_snake)
					create_body();

				if (increasing_snake)
					delete_body();

				if (!normal_snake)
				{
					if (difficulty == 1)
						score += 2;
					else
						score += (5 * difficulty);
				}
				else
				{
					if (difficulty == 1)
						score += 1;
					else
						score += difficulty;
				}
			}
			break;
		case 3:
			if ((head->x) > (food_x) && head->x < food_x + 32 && head->y + 16 > food_y && head->y + 16 < (food_y + 32))
			{
				food_flag = 1;
				if (normal_snake || decreasing_snake)
					create_body();

				if (increasing_snake)
					delete_body();

				if (!normal_snake)
				{
					if (difficulty == 1)
						score += 2;
					else
						score += (5 * difficulty);
				}
				else
				{
					if (difficulty == 1)
						score += 1;
					else
						score += difficulty;
				}
			}
			break;
		}

	while (trav)
	{

		if (head->x == trav->x && head->y == trav->y)
		{
			gameover();
			ext = 1;
		}
		trav = trav->next;
	}
}

void place_food()
{

	int flg = 1;
	snake_body* trav;
	while (flg)
	{
		trav = head;
		flg = 0;
		food_x = rand() % (SCREEN_W - 64);
		food_y = rand() % (SCREEN_H - 64);
		while (trav)
		{
			if (food_x == trav->x && food_y == trav->y)
			{
				flg = 1; break;
			}

			trav = trav->next;
		}
	}


}

void normal_boring_snake()
{
	int rev_count = 2 * difficulty, rev_count1 = 5;
	rev_flag = 0;
	normal_snake = 1;

	create_body();
	create_body();
	create_body();
	create_body();

	while (!ext)
	{


		if (!rev_count)
		{
			rev_flag = 0;
			rev_count = 2 * difficulty;
		}




		get_input();


		move_snake();
		rest(100 - 25 * difficulty);
		if (food_flag)
		{
			place_food();
			food_flag = 0;
			if (rev_flag)
				rev_count--;

			rev_count1--;

			if (!rev_count1)
			{
				rev_flag = 1; rev_count1 = 10 - difficulty;
			}
		}
		display();
		check_collision();

		if (key[KEY_ESC])
			pause_game();

	}
	destry();
}

void cool_decrease_snake()
{
	decreasing_snake = 1;
	decrease_count = 50 - (10 * difficulty);
	for (int i = 0; i < 5; i++)
		create_body();

	while (!ext)
	{
		get_input();

		move_snake();
		rest(100 - (25 * difficulty));
		if (food_flag)
		{
			place_food();
			food_flag = 0;
		}
		if (!decrease_count)
		{
			decrease_count = 50 - (10 * difficulty);
			delete_body();
		}
		display();
		check_collision();
		if (key[KEY_ESC])
			pause_game();
	}
	//destry();
}

void cool_increase_snake()
{
	increasing_snake = 1;

	for (int i = 0; i < 5; i++)
		create_body();

	while (!ext)
	{

		get_input();
		move_snake();
		rest(100 - (25 * difficulty));
		if (food_flag)
		{
			place_food();
			food_flag = 0;
		}
		if (!increase_count)
		{
			increase_count = (30 - 5 * difficulty);
			create_body();
		}
		display();
		check_collision();
		if (key[KEY_ESC])
			pause_game();
	}
	destry();
}

void Mainmenu()
{
	BITMAP* bmp = load_bitmap("resources/background1.bmp", NULL);
	f = load_font("resources/Andy.pcx", NULL, NULL);
	f2 = load_font("resources/Sybil.pcx", NULL, NULL);

	int pos = 1, n;
	ext = 0;
	while (1)
	{
		blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		textprintf_ex(buffer, f2, 480, 100, WHITE, -1, "The Snake");
		textprintf_ex(buffer, f, 180, 355, GREY, -1, "I Wanna Play The Game");
		textprintf_ex(buffer, f, 210, 385, GREY, -1, "I'm Bored Get me out of Here");

		switch (pos)
		{
		case 1:
			textprintf_ex(buffer, f, 180, 355, WHITE, -1, "I Wanna Play The Game");
			break;
		case 2:
			textprintf_ex(buffer, f, 210, 385, WHITE, -1, "I'm Bored Get me out of Here");
			break;
		}
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		rest(100);

		if (key[KEY_UP])
			pos = 1;

		if (key[KEY_DOWN])
			pos = 2;

		if (key[KEY_ENTER])
		{
			switch (pos)
			{
			case 1:
				return;

			case 2:
				f2 = load_font("resources/Sybil.pcx", NULL, NULL);
				while (!key[KEY_ESC])
				{
					blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
					textprintf_ex(buffer, f2, 460, 100, WHITE, -1, "The Snake");
					textprintf_ex(buffer, f2, 420, 270, WHITE, -1, "A Game By");
					textprintf_ex(buffer, f2, 450, 320, WHITE, -1, "Swaroop Kishen");
					blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
				}
				exit(0);
			}
		}



	}
}
void set_difficulty()
{

	BITMAP* bmp = load_bitmap("resources/background1.bmp", NULL);
	f = load_font("resources/Andy.pcx", NULL, NULL);
	f2 = load_font("resources/Sybil.pcx", NULL, NULL);


	int pos = 1, n;
	while (!ext)
	{
		blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		textprintf_ex(buffer, f2, 480, 100, WHITE, -1, "The Snake");
		textprintf_ex(buffer, f, 180, 415, GREY, -1, "I'm Scared Let me Play Safe");
		textprintf_ex(buffer, f, 210, 445, GREY, -1, "I'm Neither Scared nor Brave");
		textprintf_ex(buffer, f, 240, 475, GREY, -1, "I'm Brave Enough to handle This Snake");

		switch (pos)
		{
		case 1:
			textprintf_ex(buffer, f, 180, 415, WHITE, -1, "I'm Scared Let me Play Safe");
			break;
		case 2:
			textprintf_ex(buffer, f, 210, 445, WHITE, -1, "I'm Neither Scared nor Brave");
			break;
		case 3:
			textprintf_ex(buffer, f, 240, 475, WHITE, -1, "I'm Brave Enough to handle This Snake");
			break;
		}

		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		rest(125);

		if (key[KEY_UP])
		{
			if (pos == 1)
				pos = 1;
			else
				pos--;
		}

		else if (key[KEY_DOWN])
		{
			if (pos == 3)
				pos = 3;
			else
				pos++;
		}


		if (key[KEY_ENTER])
		{
			switch (pos)
			{
			case 1:
				difficulty = 1;
				return;
			case 2:
				difficulty = 2;
				return;
			case 3:
				difficulty = 3;
				return;

			}


		}
	}



}

void cool_snake()
{
	BITMAP* bmp = load_bitmap("resources/background1.bmp", NULL);
	f = load_font("resources/Andy.pcx", NULL, NULL);
	f2 = load_font("resources/Sybil.pcx", NULL, NULL);
	//BITMAP *bmp1=load_bitmap("inst.bmp",NULL);

	int pos = 1, n;
	while (1)
	{
		blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		textprintf_ex(buffer, f2, 480, 100, WHITE, -1, "The Snake");
		textprintf_ex(buffer, f, 180, 355, GREY, -1, "The Snake length Increases");
		textprintf_ex(buffer, f, 210, 385, GREY, -1, "The Snake length Decreases");

		switch (pos)
		{
		case 1:
			textprintf_ex(buffer, f, 180, 355, WHITE, -1, "The Snake length Increases");
			break;
		case 2:
			textprintf_ex(buffer, f, 210, 385, WHITE, -1, "The Snake length Decreases");
			break;
		}
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		rest(100);

		if (key[KEY_UP])
			pos = 1;

		if (key[KEY_DOWN])
			pos = 2;

		if (key[KEY_ENTER])
		{
			switch (pos)
			{
			case 1:
				cool_increase_snake();
				return;

			case 2:
				cool_decrease_snake();
				return;

			}
		}



	}
}

void Gamescreen()
{
	BITMAP* bmp = load_bitmap("resources/background1.bmp", NULL);
	f = load_font("resources/Andy.pcx", NULL, NULL);
	f2 = load_font("resources/Sybil.pcx", NULL, NULL);


	int pos = 1, n;
	while (!ext)
	{
		blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		textprintf_ex(buffer, f2, 480, 100, WHITE, -1, "The Snake");
		textprintf_ex(buffer, f, 180, 415, GREY, -1, "Let Me Play the Normal Boring Snake");
		textprintf_ex(buffer, f, 210, 445, GREY, -1, "Let Me Play the New type of Snake");
		textprintf_ex(buffer, f, 240, 475, GREY, -1, "Take me to the Previous Screen");

		switch (pos)
		{
		case 1:
			textprintf_ex(buffer, f, 180, 415, WHITE, -1, "Let Me Play the Normal Boring Snake");
			break;
		case 2:
			textprintf_ex(buffer, f, 210, 445, WHITE, -1, "Let Me Play the New type of Snake");
			break;
		case 3:
			textprintf_ex(buffer, f, 240, 475, WHITE, -1, "Take me to the Previous Screen");
			break;
		}

		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		rest(125);

		if (key[KEY_UP])
		{
			if (pos == 1)
				pos = 1;
			else
				pos--;
		}

		else if (key[KEY_DOWN])
		{
			if (pos == 3)
				pos = 3;
			else
				pos++;
		}


		if (key[KEY_ENTER])
		{
			switch (pos)
			{
			case 1:
				normal_boring_snake();
				break;
			case 2:
				cool_snake();
				break;
			case 3:
				return;




			}
		}



	}
}

int main()
{
	initialize();
	head_img[0] = load_bitmap("resources/snake_head_up.bmp", NULL);
	head_img[1] = load_bitmap("resources/snake_head_right.bmp", NULL);
	head_img[2] = load_bitmap("resources/snake_head_down.bmp", NULL);
	head_img[3] = load_bitmap("resources/snake_head_left.bmp", NULL);

	tail_img[0] = load_bitmap("resources/snake_tail_up.bmp", NULL);
	tail_img[1] = load_bitmap("resources/snake_tail_right.bmp", NULL);
	tail_img[2] = load_bitmap("resources/snake_tail_down.bmp", NULL);
	tail_img[3] = load_bitmap("resources/snake_tail_left.bmp", NULL);

	body_img[0] = load_bitmap("resources/snake_body_up.bmp", NULL);
	body_img[1] = load_bitmap("resources/snake_body_right.bmp", NULL);
	body_img[2] = load_bitmap("resources/snake_body_down.bmp", NULL);
	body_img[3] = load_bitmap("resources/snake_body_left.bmp", NULL);

	food = load_bitmap("resources/food.bmp", NULL);
	while (1)
	{

		Mainmenu();
		set_difficulty();
		Gamescreen();
	}

	gameover();

	return 0;
}

END_OF_MAIN()

