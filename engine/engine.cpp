﻿#include "engine.hpp"

// ---=== Variables ===---

winsize Engine::terminalSize;
std::vector<std::vector<Engine::Cell>> Engine::image;
std::string Engine::stringImage;

// ---=== Functions ===---

Engine::Object* Engine::StoreObject(Object object)
{
	storedObjects.push_back(object);
	return &storedObjects[storedObjects.size() - 1];
}
Engine::Layer* Engine::StoreLayer(Layer layer)
{
	storedLayers.push_back(layer);
	return &storedLayers[storedLayers.size() - 1];
}

// This is for a future update
void BetterPrint()
{
	/*
	Options:

		1) Single string
			1.1) Write to stringImage according to terminalSize (print whatever there is)
		2) String for each row
			2.1) Write to stringImage according to imageSize, print according to terminalSize
				2.1.1) 2D Meta data vector stating where each chacater is (to know until where to print)
				2.1.2) In printing, scan the line and figure out where each character is
			2.2) Write to stringImage according to terminalSize
				2.2.1)
		3) String for each pixel, write to stringImage according to terminalSize.

		Ok, so it makes sense to write to stringImage according to terminalSize in order to save processings.
		stringImage though is kept at the maximum image size, because terminalSize is likely to change.

		The issue with writing according to terminalSize is the stuttering when the terminal resizes, which are caused beacuse
		the print isn't fast enough, so dark gaps are visible.
	*/
}

void Engine::Print()
{
	// Get terminal size
	ioctl(fileno(stdout), TIOCGWINSZ, &Engine::terminalSize);

	// Obtain the maximum length for the stringImage
	if (image.size() == 0)
		return;

	// Resize the stringImage if needed
	unsigned maxStringSize = image.size() * 3; // reserved for '\n'
	for (unsigned y = 0; y < image.size(); y++)
		maxStringSize += image[y].size() * 39; // reserved for characters
	if (maxStringSize == 0)
		return;
	if (stringImage.size() < maxStringSize) // resize
		stringImage.resize(maxStringSize);

	// Write the image to stringImage
	unsigned int l = 0;
	unsigned char lfr = 0, lfg = 0, lfb = 0, lbr = 0, lbg = 0, lbb = 0;

	// "&& y < size.ws_row" - fit into the terminal, in the case that it is too small
	// "&& printRequests == 1" - stop working on a print if there is a newer print request
	for (unsigned y = 0; y < image.size() && y < terminalSize.ws_row; y++)
	{
		if (y != 0) {
			stringImage[l] = '\n';
			l++;
		}
		// First char in the line (optimizations)
		lfr = image[y][0].f.r;
		lfg = image[y][0].f.g;
		lfb = image[y][0].f.b;

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
		lbr = image[y][0].b.r;
		lbg = image[y][0].b.g;
		lbb = image[y][0].b.b;
		stringImage[l + 19] = '\033';
		stringImage[l + 20] = '[';
		stringImage[l + 21] = '4';
		stringImage[l + 22] = '8';
		stringImage[l + 23] = ';';
		stringImage[l + 24] = '2';
		stringImage[l + 25] = ';';
		stringImage[l + 26] = lbr / 100 + '0';
		stringImage[l + 27] = (lbr % 100) / 10 + '0';
		stringImage[l + 28] = lbr % 10 + '0';
		stringImage[l + 29] = ';';
		stringImage[l + 30] = lbg / 100 + '0';
		stringImage[l + 31] = (lbg % 100) / 10 + '0';
		stringImage[l + 32] = lbg % 10 + '0';
		stringImage[l + 33] = ';';
		stringImage[l + 34] = lbb / 100 + '0';
		stringImage[l + 35] = (lbb % 100) / 10 + '0';
		stringImage[l + 36] = lbb % 10 + '0';
		stringImage[l + 37] = 'm';
		if (image[y][0].c >= ' ' && image[y][0].c <= '~')
			stringImage[l + 38] = image[y][0].c;
		else
			stringImage[l + 38] = '?';
		l += 39;
		for (unsigned x = 1; x < image[y].size() && x < terminalSize.ws_col; x++)
		{
			// foreground
			if ((image[y][x].c != ' ') && (image[y][x].f.r != lfr || image[y][x].f.g != lfg || image[y][x].f.b != lfb))
			{
				lfr = image[y][x].f.r;
				lfg = image[y][x].f.g;
				lfb = image[y][x].f.b;
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
			// background
			if (image[y][x].b.r != lbr || image[y][x].b.g != lbg || image[y][x].b.b != lbb)
			{
				lbr = image[y][x].b.r;
				lbg = image[y][x].b.g;
				lbb = image[y][x].b.b;

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
			if (image[y][x].c >= ' ' && image[y][x].c <= '~')
				stringImage[l] = image[y][x].c;
			else
				stringImage[l] = '?';
			l++;
		}
		stringImage[l] = '\033';
		stringImage[l + 1] = '[';
		stringImage[l + 2] = 'm';
		l += 3;
	}
	// ".substr(0, l)" - print until the end which was set by the writing sequence above.
	std::cout << "\033[H\033[3J\033[2J" << stringImage.substr(0, l) << std::flush;
}

void Engine::ResetTerminal()
{
	// Reenable canonical mode and echo
	termios terminalAttributes;
	tcgetattr(0, &terminalAttributes);
	terminalAttributes.c_lflag |= ICANON; 
	terminalAttributes.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &terminalAttributes);
	// Show cursor and reset colors
	std::cout << "\033[?25h\033[0m" << std::flush;
}

void SignalHandler(int signal)
{
	if (signal == SIGWINCH)
	{
		// Currntly contains nothing until BetterPrint update

		// std::cout << "\033[H\033[3J\033[2J#####" << std::flush;
		// Engine::Print();
	}
	if (signal == SIGINT) // I just find it so cool these signals work :)
		Engine::running = false;
}

void Engine::PrepareTerminal(Engine::UPoint imageSize)
{
	// Hide cursor
	std::cout << "\033[?25l";

	// Disable canonical mode and echo
	termios terminalAttributes;
	tcgetattr(0, &terminalAttributes);
	terminalAttributes.c_lflag &= ~ICANON;
	terminalAttributes.c_lflag &= ~ECHO;
	terminalAttributes.c_cc[VMIN] = 1;
	terminalAttributes.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &terminalAttributes);

	// Size the image
	image.resize(imageSize.y);
	for (size_t y = 0; y < image.size(); y++)
		image[y].resize(imageSize.x);
	unsigned maxStringSize = image.size() * 4; // reserved for '\n'
	for (unsigned y = 0; y < image.size(); y++)
		maxStringSize += image[y].size() * 39; // reserved for characters
	if (maxStringSize == 0)
		return;
	if (stringImage.size() < maxStringSize)
		stringImage.resize(maxStringSize);

	// Resize signal
	signal(SIGWINCH, SignalHandler);
	// Exit signal
	signal(SIGINT, SignalHandler);
}

void Engine::Log(const std::string& text, RGB color)
{
	static uint32_t logIndex = 0;
	std::cout << "\033[38;2;" << std::to_string(color.r) << ';' << std::to_string(color.g) << ';' << std::to_string(color.b) << 'm' << logIndex << "] " << text << "\033[m" << std::endl << std::flush;
	logIndex++;
}