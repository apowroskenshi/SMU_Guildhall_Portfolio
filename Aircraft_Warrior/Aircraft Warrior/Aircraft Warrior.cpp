#include "allegro.h"
#include "mappyal.h"
//this must run at 640x480
//#define MODE GFX_AUTODETECT_FULLSCREEN
#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define WHITE makecol(255,255,255)
#define GRAY makecol(60,60,60)
#define RED makecol(200,0,0)
#define MAX_ENEMIES 20
#define MAX_BULLETS 20
#define MAX_EXPLOSIONS 10
#define MAX_HEALTH 30
#define BOTTOM 48000 - HEIGHT
//define the sprite structure
typedef struct SPRITE
{
	int dir, alive;
	int x, y;
	int width, height;
	int xspeed, yspeed;
	int xdelay, ydelay;
	int xcount, ycount;
	int curframe, maxframe, animdir;
	int framecount, framedelay;
}SPRITE;
//y offset in pixels
int yoffset = BOTTOM;
//player variables
int firecount = 0;
int firedelay = 60;
int health = 150;
int maxhealth = 150;
int minibosshealth;
int score = 0;
int bosshealth = 100;
int bulletcnt = 0;
int minibosscount = 1;
int powerCount = 0, sheildCount = 0;
int Cnt = 1, PCnt = 1;
long int noOfPower = 0, enemiesDestroyed = 0;
//timer variables
volatile int counter;
volatile int ticks;
volatile int framerate;
//bitmaps and sprites
BITMAP* buffer;
BITMAP* temp;
BITMAP* explosion_images[6];
SPRITE* explosions[MAX_EXPLOSIONS];
BITMAP* bigexp_images[7];
SPRITE* bigexp;
BITMAP* player_images[3];
SPRITE* player;
BITMAP* bullet_images[2];
SPRITE* bullets[MAX_BULLETS];
BITMAP* enemy_plane_images[3];
SPRITE* enemy_planes[MAX_ENEMIES];
BITMAP* progress, * bar;
BITMAP* miniboss_image;
SPRITE* miniboss;
SPRITE* bullet_miniboss[3][MAX_ENEMIES];
BITMAP* boss_image;
SPRITE* boss;
SPRITE* bullet_boss[5][MAX_BULLETS];
BITMAP* bossbullet;
BITMAP* boss_missile;
SPRITE* missile[MAX_BULLETS];
FONT* f1;
FONT* f2, * f3;
BITMAP* miniboss_frame;
BITMAP* health_image;
SPRITE* plhealth;
BITMAP* power_image;
SPRITE* power;
BITMAP* player_bullet;
SAMPLE* lvl, * smp;
BITMAP* grabframe(BITMAP* source, int width, int height, int startx, int starty, int columns, int frame)
{
	BITMAP* temp = create_bitmap(width, height);
	int x = startx + (frame % columns) * width;
	int y = starty + (frame / columns) * height;
	blit(source, temp, x, y, 0, 0, width, height);
	return temp;
}
void loadsprites(void)
{
	int n;
	//load progress bar
	temp = load_bitmap("resources/images/progress.bmp", NULL);
	progress = grabframe(temp, 130, 14, 5, 12, 1, 0);
	bar = grabframe(temp, 6, 10, 135, 10, 1, 0);
	destroy_bitmap(temp);
	//load player airplane sprite
	health_image = load_bitmap("resources/images/sheild.bmp", NULL);
	power_image = load_bitmap("resources/images/power.bmp", NULL);
	for (n = 0; n < 3; n++)
	{
		player_images[n] = new BITMAP;
		player_images[n] = load_bitmap("resources/images/F22.bmp", NULL);
	}
	//initialize the player’s sprite
	player = (SPRITE*)malloc(sizeof(SPRITE));
	player->x = 320 - 32;
	player->y = 400;
	player->width = player_images[0]->w;
	player->height = player_images[0]->h;
	player->xdelay = 1000;
	player->ydelay = 1000;
	player->xcount = 0;
	player->ycount = 0;
	player->xspeed = 0;
	player->yspeed = 0;
	player->curframe = 0;
	player->maxframe = 2;
	player->framecount = 0;
	player->framedelay = 10;
	player->animdir = 0;
	//load bullet images
	bullet_images[0] = new BITMAP;
	bullet_images[0] = load_bitmap("resources/images/bullets.bmp", NULL);
	player_bullet = new BITMAP;
	player_bullet = load_bitmap("resources/images/playerbullet.bmp", NULL);

	//initialize the bullet sprites
	for (n = 0; n < MAX_BULLETS; n++)
	{
		bullets[n] = (SPRITE*)malloc(sizeof(SPRITE));
		bullets[n]->alive = 0;
		bullets[n]->x = 0;
		bullets[n]->y = 0;
		bullets[n]->width = bullet_images[0]->w;
		bullets[n]->height = bullet_images[0]->h;
		bullets[n]->xdelay = 0;
		bullets[n]->ydelay = 5;
		bullets[n]->xcount = 0;
		bullets[n]->ycount = 0;
		bullets[n]->xspeed = 0;
		bullets[n]->yspeed = -4;
		bullets[n]->curframe = 0;
		bullets[n]->maxframe = 0;
		bullets[n]->framecount = 0;
		bullets[n]->framedelay = 0;
		bullets[n]->animdir = 0;
	}
	//load enemy plane sprites
	for (n = 0; n < 3; n++)
	{
		enemy_plane_images[n] = new BITMAP;
		enemy_plane_images[n] = load_bitmap("resources/images/enemy.bmp", NULL);
	}
	//initialize the enemy planes
	for (n = 0; n < MAX_ENEMIES; n++)
	{
		enemy_planes[n] = (SPRITE*)malloc(sizeof(SPRITE));
		enemy_planes[n]->alive = 0;
		enemy_planes[n]->x = rand() % 100 + 50;
		enemy_planes[n]->y = 0;
		enemy_planes[n]->width = enemy_plane_images[0]->w;
		enemy_planes[n]->height = enemy_plane_images[0]->h;
		enemy_planes[n]->xdelay = 20;
		enemy_planes[n]->ydelay = 20;
		enemy_planes[n]->xcount = 0;
		enemy_planes[n]->ycount = 0;
		enemy_planes[n]->xspeed = (rand() % 2 - 3);
		enemy_planes[n]->yspeed = 1;
		enemy_planes[n]->curframe = 0;
		enemy_planes[n]->maxframe = 2;
		enemy_planes[n]->framecount = 0;
		enemy_planes[n]->framedelay = 10;
		enemy_planes[n]->animdir = 1;
	}
	//load explosion sprites
	temp = load_bitmap("resources/images/explosion.bmp", NULL);
	for (n = 0; n < 6; n++)
	{
		explosion_images[n] = new BITMAP;
		explosion_images[n] = grabframe(temp, 32, 32, 0, 0, 6, n);
	}
	destroy_bitmap(temp);
	//initialize the sprites
	for (n = 0; n < MAX_EXPLOSIONS; n++)
	{
		explosions[n] = (SPRITE*)malloc(sizeof(SPRITE));
		explosions[n]->alive = 0;
		explosions[n]->x = 0;
		explosions[n]->y = 0;
		explosions[n]->width = explosion_images[0]->w;
		explosions[n]->height = explosion_images[0]->h;
		explosions[n]->xdelay = 0;
		explosions[n]->ydelay = 8;
		explosions[n]->xcount = 0;
		explosions[n]->ycount = 0;
		explosions[n]->xspeed = 0;
		explosions[n]->yspeed = -1;
		explosions[n]->curframe = 0;
		explosions[n]->maxframe = 5;
		explosions[n]->framecount = 0;
		explosions[n]->framedelay = 15;
		explosions[n]->animdir = 1;
	}
	//load explosion sprites
	temp = load_bitmap("resources/images/bigexplosion.bmp", NULL);
	for (n = 0; n < 8; n++)
	{
		bigexp_images[n] = new BITMAP;
		bigexp_images[n] = grabframe(temp, 64, 64, 0, 0, 7, n);
	}
	destroy_bitmap(temp);
	//initialize the sprites
	bigexp = (SPRITE*)malloc(sizeof(SPRITE));
	bigexp->alive = 0;
	bigexp->x = 0;
	bigexp->y = 0;
	bigexp->width = bigexp_images[0]->w;
	bigexp->height = bigexp_images[0]->h;
	bigexp->xdelay = 0;
	bigexp->ydelay = 8;
	bigexp->xcount = 0;
	bigexp->ycount = 0;
	bigexp->xspeed = 0;
	bigexp->yspeed = -1;
	bigexp->curframe = 0;
	bigexp->maxframe = 6;
	bigexp->framecount = 0;
	bigexp->framedelay = 10;
	bigexp->animdir = 1;

	miniboss_image = load_bitmap("resources/images/Ye-355A.bmp", NULL);

	//initialize the player’s sprite
	miniboss = (SPRITE*)malloc(sizeof(SPRITE));
	miniboss->alive = 0;
	miniboss->x = 320 - 80;
	miniboss->y = 0;
	miniboss->width = miniboss_image->w;
	miniboss->height = miniboss_image->h;
	miniboss->xdelay = 50;
	miniboss->ydelay = 0;
	miniboss->xcount = 0;
	miniboss->ycount = 0;
	miniboss->xspeed = 0;
	miniboss->yspeed = 0;
	miniboss->curframe = 0;
	miniboss->maxframe = 2;
	miniboss->framecount = 0;
	miniboss->framedelay = 20;
	miniboss->animdir = 0;

	for (int k = 0; k < 3; k++)
		for (int n = 0; n < MAX_BULLETS; n++)
		{
			bullet_miniboss[k][n] = (SPRITE*)malloc(sizeof(SPRITE));
			bullet_miniboss[k][n]->alive = 0;
			bullet_miniboss[k][n]->x = (miniboss->x + miniboss->width) - ((k) * 33.33);
			bullet_miniboss[k][n]->y = miniboss->y + miniboss->height;
			bullet_miniboss[k][n]->width = bullet_images[0]->w;
			bullet_miniboss[k][n]->height = bullet_images[0]->h;
			bullet_miniboss[k][n]->xdelay = 0;
			bullet_miniboss[k][n]->ydelay = 5;
			bullet_miniboss[k][n]->xcount = 0;
			bullet_miniboss[k][n]->ycount = 0;
			bullet_miniboss[k][n]->xspeed = 0;
			bullet_miniboss[k][n]->yspeed = 5;
			bullet_miniboss[k][n]->curframe = 0;
			bullet_miniboss[k][n]->maxframe = 0;
			bullet_miniboss[k][n]->framecount = 0;
			bullet_miniboss[k][n]->framedelay = 0;
			bullet_miniboss[k][n]->animdir = 0;
		}
	for (int n = 0; n < MAX_ENEMIES; n++)
	{
		bullet_miniboss[0][n]->x = miniboss->x;
		bullet_miniboss[1][n]->x = miniboss->x + miniboss->width / 2;
		bullet_miniboss[2][n]->x = miniboss->x + miniboss->width;
	}
	boss_image = load_bitmap("resources/images/B-3.bmp", NULL);
	boss = (SPRITE*)malloc(sizeof(SPRITE));
	boss->alive = 0;
	boss->x = 320 - 100;
	boss->y = 0;
	boss->width = miniboss_image->w;
	boss->height = miniboss_image->h;
	boss->xdelay = 50;
	boss->ydelay = 0;
	boss->xcount = 0;
	boss->ycount = 0;
	boss->xspeed = 0;
	boss->yspeed = 0;
	boss->curframe = 0;
	boss->maxframe = 2;
	boss->framecount = 0;
	boss->framedelay = 20;
	boss->animdir = 0;

	bossbullet = load_bitmap("resources/images/bossbullet0.bmp", NULL);
	for (int k = 0; k < 5; k++)
		for (int n = 0; n < MAX_BULLETS; n++)
		{
			bullet_boss[k][n] = (SPRITE*)malloc(sizeof(SPRITE));
			bullet_boss[k][n]->alive = 0;
			bullet_boss[k][n]->x = (boss->x + boss->width) - ((k) * 20);
			bullet_boss[k][n]->y = boss->y + boss->height;
			bullet_boss[k][n]->width = 32;
			bullet_boss[k][n]->height = 32;
			bullet_boss[k][n]->xdelay = 2;
			bullet_boss[k][n]->ydelay = 2;
			bullet_boss[k][n]->xcount = 0;
			bullet_boss[k][n]->ycount = 0;
			bullet_boss[k][n]->xspeed = 0;
			bullet_boss[k][n]->yspeed = 5;
			bullet_boss[k][n]->curframe = 0;
			bullet_boss[k][n]->maxframe = 0;
			bullet_boss[k][n]->framecount = 0;
			bullet_boss[k][n]->framedelay = 0;
			bullet_boss[k][n]->animdir = 0;
		}

	boss_missile = load_bitmap("resources/images/missile.bmp", NULL);
	for (int n = 0; n < MAX_BULLETS; n++)
	{
		missile[n] = (SPRITE*)malloc(sizeof(SPRITE));
		missile[n]->alive = 0;
		missile[n]->x = boss->x + miniboss->width / 2;
		missile[n]->y = boss->y + boss->height;
		missile[n]->width = boss_missile->w;
		missile[n]->height = boss_missile->h;
		missile[n]->xdelay = 0;
		missile[n]->ydelay = 2;
		missile[n]->xcount = 0;
		missile[n]->ycount = 0;
		missile[n]->xspeed = 0;
		missile[n]->yspeed = 5;
		missile[n]->curframe = 0;
		missile[n]->maxframe = 0;
		missile[n]->framecount = 0;
		missile[n]->framedelay = 0;
		missile[n]->animdir = 0;
	}


	plhealth = (SPRITE*)malloc(sizeof(SPRITE));
	plhealth->x = 0;
	plhealth->y = 0;
	plhealth->alive = 0;
	plhealth->width = 32;
	plhealth->height = 32;
	plhealth->xdelay = 0;
	plhealth->ydelay = 10;
	plhealth->xcount = 0;
	plhealth->ycount = 0;
	plhealth->xspeed = 0;
	plhealth->yspeed = 2;
	plhealth->curframe = 0;
	plhealth->maxframe = 0;
	plhealth->framecount = 0;
	plhealth->framedelay = 0;
	plhealth->animdir = 0;


	power = (SPRITE*)malloc(sizeof(SPRITE));
	power->x = 0;
	power->y = 0;
	power->alive = 0;
	power->width = 32;
	power->height = 32;
	power->xdelay = 0;
	power->ydelay = 10;
	power->xcount = 0;
	power->ycount = 0;
	power->xspeed = 0;
	power->yspeed = 2;
	power->curframe = 0;
	power->maxframe = 0;
	power->framecount = 0;
	power->framedelay = 0;
	power->animdir = 0;

}
int inside(int x, int y, int left, int top, int right, int bottom)
{
	if (x > left && x < right && y > top && y < bottom)
		return 1;
	else
		return 0;
}
void updatesprite(SPRITE* spr)
{
	//update x position
	if (++spr->xcount > spr->xdelay)
	{
		spr->xcount = 0;
		spr->x += spr->xspeed;
	}
	//update y position
	if (++spr->ycount > spr->ydelay)
	{
		spr->ycount = 0;
		spr->y += spr->yspeed;
	}
	//update frame based on animdir
	if (++spr->framecount > spr->framedelay)
	{
		spr->framecount = 0;
		if (spr->animdir == -1)
		{
			if (--spr->curframe < 0)
				spr->curframe = spr->maxframe;
		}
		else if (spr->animdir == 1)
		{
			if (++spr->curframe > spr->maxframe)
				spr->curframe = 0;
		}
	}
}
void startexplosion(int x, int y)
{
	int n;
	for (n = 0; n < MAX_EXPLOSIONS; n++)
	{
		if (!explosions[n]->alive)
		{
			explosions[n]->alive++;
			explosions[n]->x = x;
			explosions[n]->y = y;
			break;
		}
	}
}
void updateexplosions()
{
	int n, c = 0;
	for (n = 0; n < MAX_EXPLOSIONS; n++)
	{
		if (explosions[n]->alive)
		{
			c++;
			updatesprite(explosions[n]);
			draw_sprite(buffer, explosion_images[explosions[n]->curframe], explosions[n]->x, explosions[n]->y);
			if (explosions[n]->curframe >= explosions[n]->maxframe)
			{
				explosions[n]->curframe = 0;
				explosions[n]->alive = 0;
			}
		}
	}
	//update the big "player" explosion if needed
	if (bigexp->alive)
	{
		updatesprite(bigexp);
		draw_sprite(buffer, bigexp_images[bigexp->curframe], bigexp->x, bigexp->y);
		if (bigexp->curframe >= bigexp->maxframe)
		{
			bigexp->curframe = 0;
			bigexp->alive = 0;
		}
	}
}
void updatebullet(SPRITE* spr)
{
	int n, x, y;

	int x1, y1, x2, y2;
	//move the bullet
	updatesprite(spr);
	//check bounds
	if (spr->y < 0)
	{
		spr->alive = 0;
		return;
	}
	for (n = 0; n < MAX_ENEMIES; n++)
	{
		if (enemy_planes[n]->alive)
		{
			//find center of bullet
			x = spr->x + spr->width / 2;
			y = spr->y + spr->height / 2;
			//get enemy plane bounding rectangle
			x1 = enemy_planes[n]->x;
			y1 = enemy_planes[n]->y - yoffset;
			x2 = x1 + enemy_planes[n]->width;
			y2 = y1 + enemy_planes[n]->height;
			//check for collisions
			if (inside(x, y, x1, y1, x2, y2))
			{
				enemy_planes[n]->alive = 0;
				spr->alive = 0;
				startexplosion(spr->x + 16, spr->y);
				enemiesDestroyed++;
				powerCount++;
				sheildCount++;
				if (sheildCount == 30)
				{
					plhealth->x = enemy_planes[n]->x;
					plhealth->y = enemy_planes[n]->y - yoffset;
				}
				if (powerCount == 45)
				{
					power->x = enemy_planes[n]->x;
					power->y = enemy_planes[n]->y - yoffset;
				}
				score += 2;
				break;
			}
		}
	}
	//check for mini boss
	if (miniboss->alive)
	{
		//find center of bullet
		x = spr->x + spr->width / 2;
		y = spr->y + spr->height / 2;
		//get enemy plane bounding rectangle
		x1 = 320 - 80;
		y1 = 0;
		x2 = x1 + miniboss->width;
		y2 = y1 + miniboss->height;
		//check for collisions
		if (inside(x, y, x1, y1, x2, y2))
		{
			minibosshealth--;
			spr->alive = 0;

			startexplosion(spr->x + 16, spr->y);
			score += 2;
		}
	}
	//check for boss
	if (boss->alive)
	{
		//find center of bullet
		x = spr->x + spr->width / 2;
		y = spr->y + spr->height / 2;
		//get enemy plane bounding rectangle
		x1 = 320 - 100;
		y1 = 0;
		x2 = x1 + boss->width;
		y2 = y1 + boss->height;
		//check for collisions
		if (inside(x, y, x1, y1, x2, y2))
		{
			bosshealth -= 1;
			spr->alive = 0;

			startexplosion(spr->x, spr->y);
			score += 2;
		}
	}

}
void updatebullets()
{
	int n;
	//update/draw bullets
	for (n = 0; n < MAX_BULLETS; n++)
		if (bullets[n]->alive)
		{
			updatebullet(bullets[n]);
			draw_sprite(buffer, player_bullet, bullets[n]->x, bullets[n]->y);
		}
}

void placepowerups()
{
	if (sheildCount > 30)
	{
		plhealth->alive = 1; sheildCount = 0;
	}
	if (plhealth->alive)
	{
		updatesprite(plhealth);
		if (plhealth->x < 0)
			plhealth->x = 0;
		else if (plhealth->x > SCREEN_W - plhealth->width)
			plhealth->x = SCREEN_W - plhealth->width;
		if (plhealth->y < 480)
			draw_sprite(buffer, health_image, plhealth->x, plhealth->y);
		else
			plhealth->alive = 0;


	}
	if (powerCount > 45)
	{
		power->alive = 1; powerCount = 0;
	}
	if (power->alive)
	{
		updatesprite(power);
		if (power->x < 0)
			power->x = 0;
		else if (power->x > SCREEN_W - power->width)
			power->x = SCREEN_W - power->width;

		if (power->y < 480)
			draw_sprite(buffer, power_image, power->x, power->y);
		else
			power->alive = 0;  Cnt = 0;
	}
}
void bouncex_warpy(SPRITE* spr)
{
	//bounces x off bounds
	if (spr->x < 0)
	{
		spr->x = 0;
		spr->xspeed *= -1;
	}
	else if (spr->x > SCREEN_W - spr->width)
	{
		spr->x = SCREEN_W - spr->width;
		spr->xspeed *= -1;
	}
	//warps y if plane has passed the player
	if (spr->y > yoffset + 2000)
	{
		//respawn enemy plane
		spr->y = yoffset - 1000 - rand() % 1000;
		spr->alive++;
		spr->x = rand() % WIDTH;
	}
	//warps y from bottom to top of level
	if (spr->y < 0)
	{
		spr->y = 0;
	}
	else if (spr->y > 48000)
	{
		spr->y = 0;
	}
}
void fireatenemy()
{
	int n;
	for (n = 0; n < MAX_BULLETS; n++)
	{
		if (!bullets[n]->alive)
		{
			bullets[n]->alive++;
			bullets[n]->x = player->x + 15;
			bullets[n]->y = player->y;
			return;
		}
	}
}
void displayprogress(int life)
{
	int n;
	draw_sprite(buffer, progress, 490, 15);
	for (n = 0; n < life / 6; n++)
		draw_sprite(buffer, bar, 492 + n * 5, 15);
}
void updateenemyplanes()
{
	int n, c = 0;
	//update/draw enemy planes
	for (n = 0; n < MAX_ENEMIES; n++)
	{
		if (enemy_planes[n]->alive)
		{
			c++;
			updatesprite(enemy_planes[n]);
			bouncex_warpy(enemy_planes[n]);
			//is plane visible on screen?
			if (enemy_planes[n]->y > yoffset - 32 &&
				enemy_planes[n]->y < yoffset + HEIGHT + 32)
			{
				//draw enemy plane
				draw_sprite(buffer,
					enemy_plane_images[0],
					enemy_planes[n]->x, enemy_planes[n]->y - yoffset);
			}
		}
		//reset plane
		else
		{
			enemy_planes[n]->alive++;
			enemy_planes[n]->x = rand() % 100 + 50;
			enemy_planes[n]->y = yoffset - 2000 + rand() % 2000;
		}
	}
}
void updatescroller()
{
	static int i = 0;
	//make sure it doesn’t scroll beyond map edge
	if (yoffset > BOTTOM) yoffset = BOTTOM;
	//scroll map up 1 pixel
	if (i > 10)
	{
		i = 0; yoffset -= 1;
	}
	i++;
	//draw map with single layer
	MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
}
void updateplayer(int a)
{
	int g, h;
	int n, x, y, x1, y1, x2, y2;
	//update/draw player sprite
	updatesprite(player);

	//check for powerups
	x = player->x + player->width / 2;
	y = player->y + player->height / 2;

	if (plhealth->alive)
	{
		x1 = plhealth->x;
		y1 = plhealth->y;
		x2 = x1 + plhealth->width;
		y2 = y1 + plhealth->height;
		if (inside(x, y, x1, y1, x2, y2))
		{
			noOfPower++;
			plhealth->alive = 0;
			health += 5;
			if (health > maxhealth)
				health = maxhealth;
		}
	}

	if (power->alive)
	{
		x1 = power->x;
		y1 = power->y;
		x2 = x1 + power->width;
		y2 = y1 + power->height;
		if (inside(x, y, x1, y1, x2, y2))
		{
			power->alive = 0;
			noOfPower++;
			bulletcnt++;
			if (bulletcnt < 4)
			{
				switch (bulletcnt)
				{
				case 1:
					destroy_bitmap(player_bullet);
					player_bullet = load_bitmap("resources/images/playerbullet1.bmp", NULL);
					break;
				case 2:
					destroy_bitmap(player_bullet);
					player_bullet = load_bitmap("resources/images/playerbullet2.bmp", NULL);
					break;
				case 3:
					destroy_bitmap(player_bullet);
					player_bullet = load_bitmap("resources/images/playerbullet3.bmp", NULL);
					break;
				}
			}
		}
	}
	//check for collision with enemy planes
	if (a)
	{
		if (miniboss->alive)
		{
			h = player->y;
			if (h <= miniboss->y + miniboss->height + 10)
				player->y = miniboss->y + miniboss->height + 10;
		}
		if (boss->alive)
		{
			g = player->y;
			if (g <= boss->y + boss->height + 20)
				player->y = boss->y + boss->height + 20;
		}
	}
	draw_sprite(buffer, player_images[0], player->x, player->y);
	x = player->x + player->width / 2;
	y = player->y + player->height / 2;
	for (n = 0; n < MAX_ENEMIES; n++)
	{
		if (enemy_planes[n]->alive)
		{
			x1 = enemy_planes[n]->x;
			y1 = enemy_planes[n]->y - yoffset;
			x2 = x1 + enemy_planes[n]->width;
			y2 = y1 + enemy_planes[n]->height;
			if (inside(x, y, x1, y1, x2, y2))
			{
				enemy_planes[n]->alive = 0;
				enemiesDestroyed++;
				if (health > 0) health -= 1;
				bigexp->alive++;
				bigexp->x = player->x;
				bigexp->y = player->y;
				score++;
			}
		}
	}
}
void displaystats()
{
	//display some status information
	/*textprintf_ex(buffer,font,0,410,WHITE,-1,"Health %d", health);
	textprintf_ex(buffer,font,0,420,WHITE,-1,"Yoffset %d", yoffset);*/
	//display score
	//textprintf_ex(buffer,font,22,22,GRAY,-1,"SCORE: %d", score);
	textprintf_ex(buffer, font, 20, 20, RED, -1, "SCORE: %d", score);
}
void checkinput()
{
	//check for keyboard input
	if (key[KEY_UP])
	{
		player->y -= 1;
		if (player->y < 100)
			player->y = 100;
	}
	if (key[KEY_DOWN])
	{
		player->y += 1;
		if (player->y > HEIGHT - 65)
			player->y = HEIGHT - 65;
	}
	if (key[KEY_LEFT])
	{
		player->x -= 1;
		if (player->x < 0)
			player->x = 0;
	}
	if (key[KEY_RIGHT])
	{
		player->x += 1;
		if (player->x > WIDTH - 65)
			player->x = WIDTH - 65;
	}
	if (key[KEY_F])
	{
		if (firecount > firedelay)
		{
			firecount = 0;
			fireatenemy();
		}
		//play_sample(bul,200,128,1000,0);
	}
}
void gameover()
{
	BITMAP* gameover = load_bitmap("resources/images/gameover.bmp", NULL);
	bigexp->alive = 1;
	startexplosion(player->x, player->y);
	SAMPLE* gmov = load_sample("resources/audio/gameover.wav");
	stop_sample(lvl);
	play_sample(gmov, 100, 128, 1000, 1);
	rest(300);
	while (!key[KEY_ESC])
	{
		blit(gameover, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		textprintf_ex(buffer, f2, 100, 150, WHITE, -1, "DISTANCE TRAVELLED    %d M", (yoffset - 10000) / 10);
		textprintf_ex(buffer, f2, 100, 180, WHITE, -1, "POWERUPS COLLECTED    %d", noOfPower);
		textprintf_ex(buffer, f2, 100, 210, WHITE, -1, "ENEMIES DESTROYED     %d", enemiesDestroyed);
		textprintf_ex(buffer, f2, 100, 240, WHITE, -1, "SCORE                 %d", score);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
	}
	stop_sample(gmov);
}

void gameend()
{
	BITMAP* blacky = load_bitmap("resources/images/black.bmp", NULL);
	SAMPLE* gmend = load_sample("resources/audio/gameend.wav");
	play_sample(gmend, 100, 128, 1000, 1);
	while (!key[KEY_ESC])
	{
		blit(blacky, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		textprintf_ex(buffer, f2, 170, 40, RED, -1, "MISSION SUCESS");
		textprintf_ex(buffer, f2, 100, 150, WHITE, -1, "DISTANCE TRAVELLED   %d M", (yoffset - 10000) / 10);
		textprintf_ex(buffer, f2, 100, 180, WHITE, -1, "POWERUPS COLLECTED   %d", noOfPower);
		textprintf_ex(buffer, f2, 100, 210, WHITE, -1, "ENEMIES DESTROYED    %d", enemiesDestroyed);
		textprintf_ex(buffer, f2, 100, 240, WHITE, -1, "SCORE                %d", score);
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
	}
	destroy_sample(gmend);
}

void initialize()
{
	//initialize program
	allegro_init();
	install_timer();
	install_keyboard();
	set_color_depth(16);
	//reserve_voices(2,-1);
	if (!detect_midi_driver(MIDI_AUTODETECT))
	{
		allegro_message("Error Sound"); exit(0);
	}
	if (!detect_digi_driver(DIGI_AUTODETECT))
	{
		allegro_message("Error Sound"); exit(0);
	}

	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL))
		allegro_message("error sound");
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
	srand(time(NULL));
	//create the double buffer and clear it
	buffer = create_bitmap(SCREEN_W, SCREEN_H);
	clear(buffer);
	//load the Mappy file
	if (MapLoad("Map.fmp") != 0)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Cannot Load Map.fmp");
		return;
	}
}

void bulletminiboss()
{
	int x, y, x1, x2, y1, y2;
	static int counter = 0;
	for (int k = 0; k < 3; k++)
		for (int n = 0; n < MAX_BULLETS; n++)
			if (!bullet_miniboss[k][n]->alive)
				bullet_miniboss[k][n]->y = miniboss->y + miniboss->height;
	for (int k = 0; k < 3; k++)
	{
		for (int n = 0; n < MAX_BULLETS; n++)
		{
			bullet_miniboss[k][n]->alive = 1;
			updatesprite(bullet_miniboss[k][n]);
			if (bullet_miniboss[k][n]->y > 480)
				bullet_miniboss[k][n]->alive = 0;
			if (player->alive)
			{
				//find center of bullet
				x = bullet_miniboss[k][n]->x; //+ bullet_miniboss[k][n]->width/2;
				y = bullet_miniboss[k][n]->y; //+ bullet_miniboss[k][n]->height/2;
		//get enemy plane bounding rectangle
				x1 = player->x;
				y1 = player->y;
				x2 = x1 + player->width;
				y2 = y1 + 90;
				//check for collisions
				if (inside(x, y, x1, y1, x2, y2))
				{
					counter++;
					if (counter == 30)
					{
						health -= 1; counter = 0;
					}
					bullet_miniboss[k][n]->alive = 0;
				}
			}
			if (bullet_miniboss[k][n]->alive)
				draw_sprite(buffer, bullet_images[0], bullet_miniboss[k][n]->x, bullet_miniboss[k][n]->y);
			draw_sprite(buffer, miniboss_image, miniboss->x, miniboss->y);

		}
	}


}

void load_boss()
{
	f2 = load_font("resources/fonts/Font 1 Size 1.pcx", NULL, NULL);
	switch (minibosscount)
	{
	case 1:
		for (int i = 0; i < 10; i++)
		{
			MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			textprintf_ex(buffer, f2, 160, 220, RED, -1, "Prepare for Mini Boss 1");
			acquire_screen();
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			release_screen();
			rest(100);
		}
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/Ye-355A frame 1.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);

		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/Ye-355A frame 2.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/Ye-355A frame 3.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);
		enemy_plane_images[0] = load_bitmap("resources/images/enemy1.bmp", NULL);
		break;
	case 2:
		for (int i = 0; i < 10; i++)
		{
			MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			textprintf_ex(buffer, f2, 160, 220, RED, -1, "Prepare for Mini Boss 2");
			acquire_screen();
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			release_screen();
			rest(100);
		}
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/SR-91A frame 1.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);

		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/SR-91A frame 2.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/SR-91A frame 3.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);
		destroy_bitmap(enemy_plane_images[0]);
		enemy_plane_images[0] = load_bitmap("resources/images/enemy2.bmp", NULL);
		break;
	case 3:
		for (int i = 0; i < 10; i++)
		{
			MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			textprintf_ex(buffer, f2, 160, 220, RED, -1, "Prepare for Mini Boss 3");
			acquire_screen();
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			release_screen();
			rest(100);
		}
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/FAX-44 frame 1.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);

		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/FAX-44 frame 2.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/FAX-44 frame 3.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, miniboss->x, miniboss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);
		enemy_plane_images[0] = load_bitmap("resources/images/enemy3.bmp", NULL);
		break;
	case 4:
		for (int i = 0; i < 10; i++)
		{
			MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			textprintf_ex(buffer, f2, 185, 220, RED, -1, "Prepare for Boss");
			acquire_screen();
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			release_screen();
			rest(100);
		}
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/B-3 frame 1.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, boss->x, boss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);

		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/B-3 frame 2.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, boss->x, boss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		miniboss_frame = load_bitmap("resources/images/B-3 frame 3.bmp", NULL);
		draw_sprite(buffer, miniboss_frame, boss->x, boss->y);
		draw_sprite(buffer, player_images[0], player->x, player->y);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		rest(1000);
		break;
	}
}
void pause_game();
void startminiboss()
{
	miniboss->alive = 1;
	minibosshealth = 50;
	int n;
	load_boss();
	while ((health != 0) && (minibosshealth != 0))
	{
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		checkinput();
		if (key[KEY_ESC])
			pause_game();
		updateplayer(1);
		updateexplosions();
		updatebullets();
		bulletminiboss();
		textprintf_ex(buffer, font, 10, 10, WHITE, -1, "MINI BOSS HEALTH");
		draw_sprite(buffer, progress, 490, 15);
		for (n = 0; n < health / 6; n++)
			draw_sprite(buffer, bar, 492 + n * 5, 15);
		draw_sprite(buffer, progress, 20, 15);
		for (n = 0; n < minibosshealth / 2; n++)
			draw_sprite(buffer, bar, 22 + n * 5, 17);
		textprintf_ex(buffer, font, 0, 420, WHITE, -1, "HEALTH %d", health);
		textprintf_ex(buffer, font, 0, 430, WHITE, -1, "MINI BOSS HEALTH %d", minibosshealth);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		firecount++;
	}
	miniboss->alive = 0;
	minibosscount++;
}

void bulletboss()
{
	int x, y, x1, x2, y1, y2;
	static int counter = 0;
	for (int k = 0; k < 5; k++)
		for (int n = 0; n < MAX_BULLETS; n++)
			if (!bullet_boss[k][n]->alive)
			{
				bullet_boss[k][n]->y = boss->y + boss->height;
				if (bosshealth <= 50 && bosshealth > 25)
				{
					switch (k)
					{
					case 0:
						bullet_boss[0][n]->x = boss->x;
						bullet_boss[0][n]->y = boss->y + boss->height / 2;
						bullet_boss[0][n]->xspeed = -4;
						bullet_boss[0][n]->yspeed = 0;
						break;
					case 1:
						bullet_boss[1][n]->x = boss->x;
						bullet_boss[1][n]->y = boss->y + boss->height;
						bullet_boss[1][n]->xspeed = -2.5;
						bullet_boss[1][n]->yspeed = 2.5;
						break;
					case 2:
						bullet_boss[2][n]->x = boss->x + boss->width / 2;
						bullet_boss[2][n]->y = boss->y + boss->height;
						bullet_boss[2][n]->xspeed = 0;
						bullet_boss[2][n]->yspeed = 4;
						break;
					case 3:
						bullet_boss[3][n]->x = boss->x + boss->width;
						bullet_boss[3][n]->y = boss->y + boss->height;
						bullet_boss[3][n]->xspeed = 2.5;
						bullet_boss[3][n]->yspeed = 2.5;
					case 4:
						bullet_boss[4][n]->x = boss->x + boss->width;
						bullet_boss[4][n]->y = boss->y + boss->height / 2;
						bullet_boss[4][n]->xspeed = 4;
						bullet_boss[4][n]->yspeed = 0;
						break;
					}

				}
			}

	for (int k = 0; k < 5; k++)
		for (int n = 0; n < MAX_BULLETS; n++)
		{
			bullet_boss[k][n]->alive = 1;
			updatesprite(bullet_boss[k][n]);
			if (bullet_boss[k][n]->y > 480)
				bullet_boss[k][n]->alive = 0;
			if (player->alive)
			{
				//find center of bullet
				x = bullet_boss[k][n]->x; //+ bullet_miniboss[k][n]->width/2;
				y = bullet_boss[k][n]->y; //+ bullet_miniboss[k][n]->height/2;
		//get enemy plane bounding rectangle
				x1 = player->x;
				y1 = player->y;
				x2 = x1 + player->width;
				y2 = y1 + 90;
				//check for collisions
				if (inside(x, y, x1, y1, x2, y2))
				{
					counter++;
					if (counter == 40)
					{
						health -= 1; counter = 0;
					}
					bullet_boss[k][n]->alive = 0;
				}
			}
			if (bosshealth > 25)
				if (bullet_boss[k][n]->alive)
					draw_sprite(buffer, bossbullet, bullet_boss[k][n]->x, bullet_boss[k][n]->y);
			draw_sprite(buffer, boss_image, boss->x, boss->y);
		}

	if (bosshealth <= 25)
	{
		for (int n = 0; n < MAX_BULLETS; n++)
			if (!missile[n]->alive)
				missile[n]->y = boss->y + boss->height;
		for (int n = 0; n < MAX_BULLETS; n++)
		{
			missile[n]->alive = 1;
			updatesprite(missile[n]);
			if (missile[n]->y > 480)
				missile[n]->alive = 0;
			if (player->alive)
			{
				//find center of bullet
				x = missile[n]->x; //+ bullet_miniboss[k][n]->width/2;
				y = missile[n]->y; //+ bullet_miniboss[k][n]->height/2;
		//get enemy plane bounding rectangle
				x1 = player->x;
				y1 = player->y;
				x2 = x1 + player->width;
				y2 = y1 + 90;
				//check for collisions
				if (inside(x, y, x1, y1, x2, y2))
				{
					counter++;
					if (counter == 40)
					{
						health -= 5; counter = 0;
					}
					missile[n]->alive = 0;
				}
			}
			if (missile[n]->alive)
				draw_sprite(buffer, boss_missile, missile[n]->x, missile[n]->y);
			draw_sprite(buffer, boss_image, boss->x, boss->y);
		}

	}
}

void startboss()
{
	boss->alive = 1;
	int cnt = 1, n;
	load_boss();
	while (health && bosshealth)
	{
		MapDrawBG(buffer, 0, yoffset, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		if (bosshealth < 50 && cnt)
		{
			destroy_bitmap(bossbullet);
			bossbullet = load_bitmap("resources/images/bossbullet.bmp", NULL);
			cnt = 0;
		}

		checkinput();
		if (key[KEY_ESC])
			pause_game();
		updateplayer(1);
		updateexplosions();
		updatebullets();
		bulletboss();
		textprintf_ex(buffer, font, 10, 10, WHITE, -1, "BOSS HEALTH");
		draw_sprite(buffer, progress, 490, 15);
		for (n = 0; n < health / 6; n++)
			draw_sprite(buffer, bar, 492 + n * 5, 15);
		draw_sprite(buffer, progress, 20, 15);
		for (n = 0; n < bosshealth / 4; n++)
			draw_sprite(buffer, bar, 22 + n * 5, 17);
		// textprintf_ex(buffer,font,0,420,WHITE,-1,"HEALTH %d", health);
		// textprintf_ex(buffer,font,0,430,WHITE,-1,"BOSS HEALTH %d",bosshealth);
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		firecount++;
	}

	boss->alive = 0;
}
void moveup(char* str, BITMAP* bmp)
{
	f1 = load_font("resources/fonts/Font Size 1.pcx", NULL, NULL);
	char str1[100]; int k; static int m = 0;
	{for (int j = 0; j < strlen(str); j++)
		for (int i = 0; i <= j; i++)
		{
			for (k = 0; k <= i; k++)
				str1[k] = str[k];
			str1[k] = '\0';
			textout_ex(buffer, f1, str1, 50, 90 + m, makecol(0, 0, 0), -1);
			blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
			rest(0.5);
		}
	m = m + 20;
	}
}
void stry();
void mainmenu()
{
	BITMAP* bmp = load_bitmap("resources/images/title.bmp", NULL);
	f2 = load_font("resources/fonts/Font 1 Size 2.pcx", NULL, NULL);
	BITMAP* bmp1 = load_bitmap("resources/images/inst.bmp", NULL);
	SAMPLE* smp = load_sample("resources/audio/main.wav");
	int pos = 1, n;
	play_sample(smp, 200, 128, 1000, 1);
	while (1)
	{
		blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		textprintf_ex(buffer, f2, 190, 255, WHITE, -1, "PLAY GAME");
		textprintf_ex(buffer, f2, 190, 285, WHITE, -1, "INSTRUCTIONS");
		textprintf_ex(buffer, f2, 190, 315, WHITE, -1, "EXIT GAME");

		switch (pos)
		{
		case 1:
			textprintf_ex(buffer, f2, 190, 255, RED, -1, "PLAY GAME");
			break;
		case 2:
			textprintf_ex(buffer, f2, 190, 285, RED, -1, "INSTRUCTIONS");
			break;
		case 3:
			textprintf_ex(buffer, f2, 190, 315, RED, -1, "EXIT GAME");
			break;
		}
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		rest(100);

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
				stop_sample(smp);
				return;
			case 2:
				while (!key[KEY_SPACE])
				{
					blit(bmp1, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
					blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
				}
				break;
			case 3:
				stop_sample(smp);
				//delete the Mappy level
				MapFreeMem();
				//delete bitmaps
				destroy_bitmap(buffer);
				destroy_bitmap(progress);
				destroy_bitmap(bar);
				for (n = 0; n < 6; n++)
					destroy_bitmap(explosion_images[n]);
				for (n = 0; n < 3; n++)
				{
					destroy_bitmap(player_images[n]);
					destroy_bitmap(enemy_plane_images[n]);
				}
				destroy_bitmap(bullet_images[0]);
				//delete sprites
				free(player);
				for (n = 0; n < MAX_EXPLOSIONS; n++)
					free(explosions[n]);
				for (n = 0; n < MAX_BULLETS; n++)
					free(bullets[n]);
				for (n = 0; n < MAX_ENEMIES; n++)
					free(enemy_planes[n]);
				free(bigexp);
				allegro_exit();
				exit(0);
			}
		}



	}
}


void stry()
{
	int i = 0, a;
	char story[][100] = { "It is World War II          ",
						"Your Country Is Under Attack    ",
						"The Country's Defence Has Fallen ",
						"blah blah blah blah .......",
						"Just play the game"

	};
	BITMAP* bmp = load_bitmap("resources/images/back.bmp", NULL);
	SAMPLE* smp1 = load_sample("resources/audio/story.wav");
	if (!smp1)
		allegro_message("Error");
	blit(bmp, buffer, 0, 0, 0, 0, 640, 480);
	//play_midi(snd,1);
	play_sample(smp1, 100, 128, 1000, 1);
	while (!key[KEY_ESC])
	{
		moveup(story[i], bmp);

		i++;
		if (i > 4)
			break;

	}
	//stop_midi();
	destroy_sample(smp1);
}
void gameover();
void pause_game()
{
	BITMAP* bmp = load_bitmap("resources/images/title.bmp", NULL);
	int pos = 1, n;
	blit(bmp, buffer, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
	while (1)
	{
		textprintf(buffer, f2, 110, 240, WHITE, "RESUME GAME");
		textprintf(buffer, f2, 110, 270, WHITE, "EXIT GAME");
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);

		switch (pos)
		{
		case 1:
			textprintf(buffer, f2, 110, 240, RED, "RESUME GAME");
			break;
		case 2:
			textprintf(buffer, f2, 110, 270, RED, "EXIT GAME");
			break;
		}
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		rest(100);
		if (key[KEY_UP])
			if (pos == 2)
				pos--;
		if (key[KEY_DOWN])
			if (pos == 1)
				pos++;


		if (key[KEY_ENTER])
		{
			switch (pos)
			{
			case 1:
				return;
			case 2:
				if (miniboss->alive || boss->alive)
					stop_sample(smp);
				gameover();
				//delete the Mappy level
				MapFreeMem();
				//delete bitmaps
				destroy_bitmap(buffer);
				destroy_bitmap(progress);
				destroy_bitmap(bar);
				for (n = 0; n < 6; n++)
					destroy_bitmap(explosion_images[n]);
				for (n = 0; n < 3; n++)
				{
					destroy_bitmap(player_images[n]);
					destroy_bitmap(enemy_plane_images[n]);
				}
				destroy_bitmap(bullet_images[0]);
				//delete sprites
				free(player);
				for (n = 0; n < MAX_EXPLOSIONS; n++)
					free(explosions[n]);
				for (n = 0; n < MAX_BULLETS; n++)
					free(bullets[n]);
				for (n = 0; n < MAX_ENEMIES; n++)
					free(enemy_planes[n]);
				free(bigexp);
				allegro_exit();
				exit(0);
				break;
			}
		}


	}

}
int main(void)
{
	int n;
	//init game
	initialize();
	loadsprites();
	mainmenu();
	//stry();
	//main loop
	smp = load_sample("resources/audio/miniboss.wav");
	lvl = load_sample("resources/audio/level.wav");
	play_sample(lvl, 100, 128, 1000, 1);
	while (health && bosshealth)
	{
		checkinput();
		if (key[KEY_ESC])
			pause_game();
		updatescroller();
		updateplayer(0);
		updateenemyplanes();
		updatebullets();
		updateexplosions();
		placepowerups();
		displayprogress(health);
		displaystats();

		if (yoffset == 40000)
		{
			stop_sample(lvl);
			play_sample(smp, 100, 128, 1000, 1);
			startminiboss();
			for (int i = 0; i < 10; i++)
				updatescroller();
			stop_sample(smp);
			play_sample(lvl, 100, 128, 1000, 1);
		}

		if (yoffset == 30000)
		{
			destroy_bitmap(miniboss_image);
			miniboss_image = load_bitmap("resources/images/SR-91A.bmp", NULL);
			stop_sample(lvl);
			play_sample(smp, 100, 128, 1000, 1);
			startminiboss();
			for (int i = 0; i < 10; i++)
				updatescroller();
			stop_sample(smp);
			play_sample(lvl, 100, 128, 1000, 1);
		}
		if (yoffset == 20000)
		{
			destroy_bitmap(miniboss_image);
			miniboss_image = load_bitmap("resources/images/FAX-44.bmp", NULL);
			stop_sample(lvl);
			play_sample(smp, 100, 128, 1000, 1);
			startminiboss();
			for (int i = 0; i < 10; i++)
				updatescroller();
			stop_sample(smp);
			play_sample(lvl, 100, 128, 1000, 1);
		}
		if (yoffset == 10000)
		{
			destroy_sample(smp);
			smp = load_sample("resources/audio/boss.wav");
			stop_sample(lvl);
			play_sample(smp, 100, 128, 1000, 1);
			startboss();
			for (int i = 0; i < 10; i++)
				updatescroller();
			stop_sample(smp);
		}
		//blit the double buffer
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		release_screen();
		ticks++;
		firecount++;
	}


	destroy_sample(lvl);
	if (!health)
		gameover();
	if (!bosshealth)
		gameend();

	mainmenu();

	return 0;
}
END_OF_MAIN()
