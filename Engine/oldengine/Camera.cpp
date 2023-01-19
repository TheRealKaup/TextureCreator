#include "Engine.h"

Engine::Camera::Camera(Vector2D position, Vector2D resolution, std::string name)
	: pos(position), res(Vector2D(abs(resolution.x), abs(resolution.y))) {}

// <400 before, >400 after second update, >1500 expected max potential before, 1300-2000 now. (20x20)

Engine::Layer* layer;
Engine::Object* obj;
Engine::Visual* vis;

size_t y, x, i, o, l, v;
long int finalY, finalX;

void Engine::Camera::Render(std::vector<Layer*> layers)
{
	if (image.size() != res.y)
		image.resize(res.y, std::vector<Pixel>(res.x));
	for (y = 0; y < res.y; y++)
	{
		for (x = 0; x < res.x; x++)
		{
			image[y][x].frgb = { 0, 0, 0 };
			image[y][x].brgb = { 0, 0, 0 };
			image[y][x].character = ' ';
		}
	}

	for (size_t i = 0; i < layers.size(); i++)
	{
		if (!layers[i]->active)
			continue;

		layer = layers[i];

		for (v = 0; v < layer->visuals.size(); v++)
		{
			vis = layer->visuals[v];

			for (y = 0; y < vis->texture.size(); y++)
			{
				finalY = y + vis->pos.y - pos.y;

				if (finalY < 0)
					continue;
				if (finalY >= res.y)
					break;

				for (x = 0; x < vis->texture[y].size(); x++)
				{
					finalX = x + vis->pos.x - pos.x;

					if (finalX < 0)
						continue;
					if (finalX >= res.x)
						break;


					if (vis->texture[y][x].character != ' ')
					{
						image[finalY][finalX].character = vis->texture[y][x].character;
						image[finalY][finalX].frgb.r = layer->transparency * vis->texture[y][x].frgba.a * vis->texture[y][x].frgba.r + (1.0f - layer->transparency * vis->texture[y][x].frgba.a) * image[finalY][finalX].frgb.r;
						image[finalY][finalX].frgb.g = layer->transparency * vis->texture[y][x].frgba.a * vis->texture[y][x].frgba.g + (1.0f - layer->transparency * vis->texture[y][x].frgba.a) * image[finalY][finalX].frgb.g;
						image[finalY][finalX].frgb.b = layer->transparency * vis->texture[y][x].frgba.a * vis->texture[y][x].frgba.b + (1.0f - layer->transparency * vis->texture[y][x].frgba.a) * image[finalY][finalX].frgb.b;
					}
					image[finalY][finalX].brgb.r = layer->transparency * vis->texture[y][x].brgba.a * vis->texture[y][x].brgba.r + (1.0f - layer->transparency * vis->texture[y][x].brgba.a) * image[finalY][finalX].brgb.r;
					image[finalY][finalX].brgb.g = layer->transparency * vis->texture[y][x].brgba.a * vis->texture[y][x].brgba.g + (1.0f - layer->transparency * vis->texture[y][x].brgba.a) * image[finalY][finalX].brgb.g;
					image[finalY][finalX].brgb.b = layer->transparency * vis->texture[y][x].brgba.a * vis->texture[y][x].brgba.b + (1.0f - layer->transparency * vis->texture[y][x].brgba.a) * image[finalY][finalX].brgb.b;
				}
			}
		}

		for (o = 0; o < layer->objects.size(); o++)
		{
			obj = layer->objects[o];

			for (y = 0; y < obj->texture.size(); y++)
			{
				finalY = y + obj->pos.y - pos.y;

				if (finalY < 0)
					continue;
				if (finalY >= res.y)
					break;

				for (x = 0; x < obj->texture[y].size(); x++)
				{
					finalX = x + obj->pos.x - pos.x;

					if (finalX < 0)
						continue;
					if (finalX >= res.x)
						break;


					if (obj->texture[y][x].character != ' ')
					{
						image[finalY][finalX].character = obj->texture[y][x].character;
						image[finalY][finalX].frgb.r = layer->transparency * obj->texture[y][x].frgba.a * obj->texture[y][x].frgba.r + (1.0f - layer->transparency * obj->texture[y][x].frgba.a) * image[finalY][finalX].frgb.r;
						image[finalY][finalX].frgb.g = layer->transparency * obj->texture[y][x].frgba.a * obj->texture[y][x].frgba.g + (1.0f - layer->transparency * obj->texture[y][x].frgba.a) * image[finalY][finalX].frgb.g;
						image[finalY][finalX].frgb.b = layer->transparency * obj->texture[y][x].frgba.a * obj->texture[y][x].frgba.b + (1.0f - layer->transparency * obj->texture[y][x].frgba.a) * image[finalY][finalX].frgb.b;
					}
					image[finalY][finalX].brgb.r = layer->transparency * obj->texture[y][x].brgba.a * obj->texture[y][x].brgba.r + (1.0f - layer->transparency * obj->texture[y][x].brgba.a) * image[finalY][finalX].brgb.r;
					image[finalY][finalX].brgb.g = layer->transparency * obj->texture[y][x].brgba.a * obj->texture[y][x].brgba.g + (1.0f - layer->transparency * obj->texture[y][x].brgba.a) * image[finalY][finalX].brgb.g;
					image[finalY][finalX].brgb.b = layer->transparency * obj->texture[y][x].brgba.a * obj->texture[y][x].brgba.b + (1.0f - layer->transparency * obj->texture[y][x].brgba.a) * image[finalY][finalX].brgb.b;
				}
			}
		}

		if ((layer->brgba.r != 0 || layer->brgba.g != 0 || layer->brgba.b != 0) && layer->brgba.a != 0)
		{
			for (size_t y = 0; y < res.y; y++)
			{
				for (size_t x = 0; x < res.x; x++)
				{
					image[y][x].brgb.r = layer->brgba.a * layer->brgba.r + (1.0f - layer->brgba.a) * image[finalY][finalX].brgb.r;
					image[y][x].brgb.g = layer->brgba.a * layer->brgba.g + (1.0f - layer->brgba.a) * image[finalY][finalX].brgb.g;
					image[y][x].brgb.b = layer->brgba.a * layer->brgba.b + (1.0f - layer->brgba.a) * image[finalY][finalX].brgb.b;
				}
			}
		}

		if ((layer->frgba.r != 0 || layer->frgba.g != 0 || layer->frgba.b != 0) && layer->frgba.a != 0)
		{
			for (size_t y = 0; y < res.y; y++)
			{
				for (size_t x = 0; x < res.x; x++)
				{
					image[y][x].frgb.r = layer->frgba.a * layer->frgba.r + (1.0f - layer->frgba.a) * image[finalY][finalX].frgb.r;
					image[y][x].frgb.g = layer->frgba.a * layer->frgba.g + (1.0f - layer->frgba.a) * image[finalY][finalX].frgb.g;
					image[y][x].frgb.b = layer->frgba.a * layer->frgba.b + (1.0f - layer->frgba.a) * image[finalY][finalX].frgb.b;
				}
			}
		}
	}
}

void Engine::Camera::Print()
{
	if (image.size() == 0)
		return;
	if (image[0].size() == 0)
		return;

	if (stringImage.size() != res.y * res.x * 39 + res.y)
		stringImage.resize(res.y * res.x * 39 + res.y);
	
	size_t l = 0;
	unsigned char lfr = 0, lfg = 0, lfb = 0, lbr = 0, lbg = 0, lbb = 0;

	lfr = image[0][0].frgb.r;
	lfg = image[0][0].frgb.g;
	lfb = image[0][0].frgb.b;
	lbr = image[0][0].brgb.r;
	lbg = image[0][0].brgb.g;
	lbb = image[0][0].brgb.b;
	stringImage[l] = '\033';
	stringImage[l + 1] = '[';
	stringImage[l + 2] = '3';
	stringImage[l + 3] = '8';
	stringImage[l + 4] = ';';
	stringImage[l + 5] = '2';
	stringImage[l + 6] = ';';
	stringImage[l + 7] = lfr / 100 + '0';
	stringImage[l + 8] = (lfr % 100) / 10 + '0';
	stringImage[l + 9] = lfr % 10 + '0';
	stringImage[l + 10] = ';';
	stringImage[l + 11] = lfg / 100 + '0';
	stringImage[l + 12] = (lfg % 100) / 10 + '0';
	stringImage[l + 13] = lfg % 10 + '0';
	stringImage[l + 14] = ';';
	stringImage[l + 15] = lfb / 100 + '0';
	stringImage[l + 16] = (lfb % 100) / 10 + '0';
	stringImage[l + 17] = lfb % 10 + '0';
	stringImage[l + 18] = 'm';
	l += 19;
	stringImage[l] = '\033';
	stringImage[l + 1] = '[';
	stringImage[l + 2] = '4';
	stringImage[l + 3] = '8';
	stringImage[l + 4] = ';';
	stringImage[l + 5] = '2';
	stringImage[l + 6] = ';';
	stringImage[l + 7] = lbr / 100 + '0';
	stringImage[l + 8] = (lbr % 100) / 10 + '0';
	stringImage[l + 9] = lbr % 10 + '0';
	stringImage[l + 10] = ';';
	stringImage[l + 11] = lbg / 100 + '0';
	stringImage[l + 12] = (lbg % 100) / 10 + '0';
	stringImage[l + 13] = lbg % 10 + '0';
	stringImage[l + 14] = ';';
	stringImage[l + 15] = lbb / 100 + '0';
	stringImage[l + 16] = (lbb % 100) / 10 + '0';
	stringImage[l + 17] = lbb % 10 + '0';
	stringImage[l + 18] = 'm';
	l += 19;

	for (size_t y = 0; y < image.size(); y++)
	{
		for (size_t x = 0; x < image[y].size(); x++)
		{
			if (image[y][x].character != ' ')
			{
				if (image[y][x].frgb.r != lfr || image[y][x].frgb.g != lfg || image[y][x].frgb.b != lfb)
				{
					lfr = image[y][x].frgb.r;
					lfg = image[y][x].frgb.g;
					lfb = image[y][x].frgb.b;

					stringImage[l] = '\033';
					stringImage[l + 1] = '[';
					stringImage[l + 2] = '3';
					stringImage[l + 3] = '8';
					stringImage[l + 4] = ';';
					stringImage[l + 5] = '2';
					stringImage[l + 6] = ';';
					stringImage[l + 7] = lfr / 100 + '0';
					stringImage[l + 8] = (lfr % 100) / 10 + '0';
					stringImage[l + 9] = lfr % 10 + '0';
					stringImage[l + 10] = ';';
					stringImage[l + 11] = lfg / 100 + '0';
					stringImage[l + 12] = (lfg % 100) / 10 + '0';
					stringImage[l + 13] = lfg % 10 + '0';
					stringImage[l + 14] = ';';
					stringImage[l + 15] = lfb / 100 + '0';
					stringImage[l + 16] = (lfb % 100) / 10 + '0';
					stringImage[l + 17] = lfb % 10 + '0';
					stringImage[l + 18] = 'm';
					l += 19;
				}
			}
			if (image[y][x].brgb.r != lbr || image[y][x].brgb.g != lbg || image[y][x].brgb.b != lbb)
			{
				lbr = image[y][x].brgb.r;
				lbg = image[y][x].brgb.g;
				lbb = image[y][x].brgb.b;

				stringImage[l] = '\033';
				stringImage[l + 1] = '[';
				stringImage[l + 2] = '4';
				stringImage[l + 3] = '8';
				stringImage[l + 4] = ';';
				stringImage[l + 5] = '2';
				stringImage[l + 6] = ';';
				stringImage[l + 7] = lbr / 100 + '0';
				stringImage[l + 8] = (lbr % 100) / 10 + '0';
				stringImage[l + 9] = lbr % 10 + '0';
				stringImage[l + 10] = ';';
				stringImage[l + 11] = lbg / 100 + '0';
				stringImage[l + 12] = (lbg % 100) / 10 + '0';
				stringImage[l + 13] = lbg % 10 + '0';
				stringImage[l + 14] = ';';
				stringImage[l + 15] = lbb / 100 + '0';
				stringImage[l + 16] = (lbb % 100) / 10 + '0';
				stringImage[l + 17] = lbb % 10 + '0';
				stringImage[l + 18] = 'm';
				l += 19;
			}

			stringImage[l] = image[y][x].character;
			l++;
		}
		stringImage[l] = '\n';
		l++;
	}

	std::cout << stringImage.substr(0, l);
}