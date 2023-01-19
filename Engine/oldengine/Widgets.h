#pragma once
#include "Engine.h"

#define Characters_Lower 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
#define Characters_Upper 'Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M'
#define Characters_Digits '1','2','3','4','5','6','7','8','9','0'
#define Characters_Special '`','~','!','@','#','$','%','^','&','*','(',')','-','_','=','+','[','{',']','}','\\','|',';',':','\'','"',',','<','.','>','/','?'

namespace Widgets
{
	class Button
	{
	public:
		bool selected = false;
		bool down = false;

		std::function<void()> OnUp;
		std::function<void()> OnDown;
		
		Engine::RGBA notRGBA, selectedRGBA, downRGBA;

		Engine::Object obj;

	private:

		void InsideOnDown()
		{
			if (selected)
			{
				for (size_t y = 0; y < obj.texture.size(); y++)
				{
					for (size_t x = 0; x < obj.texture[y].size(); x++)
						obj.texture[y][x].frgba = downRGBA;
				}
				down = true;
				if (OnDown)
					OnDown();
			}
		}

		void InsideOnUp()
		{
			if (selected)
			{
				for (size_t y = 0; y < obj.texture.size(); y++)
				{
					for (size_t x = 0; x < obj.texture[y].size(); x++)
						obj.texture[y][x].frgba = selectedRGBA;
				}
				down = false;
				if (OnUp)
					OnUp();
			}
		}

	public:

		void Select()
		{
			if (selected == false)
			{
				for (size_t y = 0; y < obj.texture.size(); y++)
				{
					for (size_t x = 0; x < obj.texture[y].size(); x++)
						obj.texture[y][x].frgba = selectedRGBA;
				}
				selected = true;
			}
		}  

		void Deselect()
		{
			if (selected == true)
			{
				for (size_t y = 0; y < obj.texture.size(); y++)
				{
					for (size_t x = 0; x < obj.texture[y].size(); x++)
						obj.texture[y][x].frgba = notRGBA;
				}
				selected = false;
			}
		}


		Button(Engine::Layer* layer, std::function<void()> OnDown, std::function<void()> OnUp, int key = VK_RETURN, Engine::Vector2D pos = { 0, 0 }, std::string text = "Button",
			Engine::RGBA notRGBA = { 150, 150, 150, 1.0f }, Engine::RGBA selectedRGBA = { 255, 255, 255, 1.0f }, Engine::RGBA downRGBA = { 150, 150, 255, 1.0f })
			: OnUp(OnUp), OnDown(OnDown), notRGBA(notRGBA), selectedRGBA(selectedRGBA), downRGBA(downRGBA)
		{
			Engine::RegisterInputHandler(key, true, std::bind(&Button::InsideOnDown, this));
			Engine::RegisterInputHandler(key, false, std::bind(&Button::InsideOnUp, this));
			
			obj.pos = pos;

			// Texture
			obj.texture.resize(3);

			Engine::SuperChar();

			obj.texture[0].resize(text.length() + 4);
			obj.texture[0][0] = Engine::SuperChar('#', notRGBA);
			obj.texture[0][text.length() + 3] = Engine::SuperChar('#', notRGBA);
			for (size_t x = 1; x < text.length() + 3; x++)
				obj.texture[0][x] = Engine::SuperChar('-', notRGBA);
			
			obj.texture[1].resize(text.length() + 4);
			obj.texture[1][0] = Engine::SuperChar('|', notRGBA);
			obj.texture[1][1] = Engine::SuperChar(' ', notRGBA);
			obj.texture[1][text.length() + 2] = Engine::SuperChar(' ');
			obj.texture[1][text.length() + 3] = Engine::SuperChar('|');
			for (size_t x = 2; x < text.length() + 2; x++)
				obj.texture[1][x] = Engine::SuperChar(text[x - 2], notRGBA);

			obj.texture[2].resize(text.length() + 4);
			obj.texture[2][0] = Engine::SuperChar('#', notRGBA);
			obj.texture[2][text.length() + 3] = Engine::SuperChar('#', notRGBA);
			for (size_t x = 1; x < text.length() + 3; x++)
				obj.texture[2][x] = Engine::SuperChar('-', notRGBA);

			// Collider

			obj.collider.resize(3, std::vector<bool>(text.length() + 4));
			obj.colliderActive = false;
			for (size_t y = 0; y < obj.collider.size(); y++)
			{
				for (size_t x = 0; x < obj.collider[y].size(); x++)
					obj.collider[y][x] = true;
			}

			layer->AddObject(&obj);
		}
	};

	class IntInputField
	{
	public:
		int number;
		
		bool selected = false;

		int currentDigit = 0;
		
		std::function<void()> OnInsert;

		Engine::RGBA notRGBA, selectedRGBA;

		Engine::Object obj;

	private:
		unsigned int min, max;
		std::string text;
		int maxDigits = 0;
		int minDigits = 0;
		
		void InternalInsert()
		{
			if (selected)
			{
				if (Engine::lastDownVirtualKey == VK_BACK || Engine::lastDownVirtualKey == VK_DELETE)
				{
					if (currentDigit == 0)
						return;
					
					number /= 10;
					
					currentDigit--;
					obj.texture[1][2 + text.length() + currentDigit].character = ' ';

					if (OnInsert)
						OnInsert();

				}
				else if (Engine::lastDownKey >= '0' || Engine::lastDownKey <= '9')
				{
					if (currentDigit == maxDigits)
						return;

					obj.texture[1][2 + text.length() + currentDigit].character = Engine::lastDownKey;
					currentDigit++;
					
					number = number * 10 + Engine::lastDownKey - '0';
					if (OnInsert)
						OnInsert();
				}
			}
		}

	public:

		void Select()
		{
			if (selected == false)
			{
				for (size_t y = 0; y < obj.texture.size(); y++)
				{
					for (size_t x = 0; x < obj.texture[y].size(); x++)
						obj.texture[y][x].frgba = selectedRGBA;
				}
				selected = true;
			}
		}

		void Deselect()
		{
			if (selected == true)
			{
				if (number < min)
				{
					number = min;
					
					currentDigit = 0;

					size_t j = 1;
					for (size_t i = 1; i < minDigits; i++)
						j *= 10;

					for (size_t x = 0; x < minDigits; x++, j /= 10)
					{
						obj.texture[1][2 + text.length() + x].character = min % (j * 10) / j + '0';
						currentDigit++;
					}
					for (size_t x = minDigits; x < maxDigits; x++)
						obj.texture[1][2 + text.length() + x].character = ' ';
				}
				if (number > max)
				{
					number = max;
					
					currentDigit = 0;

					size_t j = 1;
					for (size_t i = 1; i < maxDigits; i++)
						j *= 10; 

					for (size_t x = 0; x < maxDigits; x++, j /= 10)
					{
						obj.texture[1][2 + text.length() + x].character = max % (j * 10) / j + '0';
						currentDigit++;
					}
				}

				for (size_t y = 0; y < obj.texture.size(); y++)
				{
					for (size_t x = 0; x < obj.texture[y].size(); x++)
						obj.texture[y][x].frgba = notRGBA;
				}
				selected = false;
			}
		}


		IntInputField(Engine::Layer* layer, std::function<void()> OnInsert, unsigned int min = 0, unsigned int max = 255, std::string defaultNum = "0", Engine::Vector2D pos = {0, 0},
			std::string text = "Value = ", Engine::RGBA notRGBA = { 150, 150, 150, 1.0f }, Engine::RGBA selectedRGBA = { 255, 255, 255, 1.0f })
			: OnInsert(OnInsert), min(min), max(max), notRGBA(notRGBA), selectedRGBA(selectedRGBA), text(text)
		{
			Engine::RegisterInputHandler('0', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('1', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('2', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('3', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('4', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('5', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('6', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('7', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('8', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler('9', true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler(VK_BACK, true, std::bind(&IntInputField::InternalInsert, this));
			Engine::RegisterInputHandler(VK_DELETE, true, std::bind(&IntInputField::InternalInsert, this));

			// *** Creating the object ***
			obj.pos = pos;
			// Texture
			obj.texture.resize(3);

			for (size_t i = 1; true; i *= 10)
			{
				if (max / i > 0)
					maxDigits++;
				else
					break;
			}
			for (size_t i = 1; true; i *= 10)
			{
				if (min / i > 0)
					minDigits++;
				else
					break;
			}

			obj.texture[0].resize(text.length() + maxDigits + 4);
			obj.texture[0][0] = Engine::SuperChar('#', notRGBA);
			obj.texture[0][text.length() + maxDigits + 3] = Engine::SuperChar('#', notRGBA);
			for (size_t x = 1; x < text.length() + maxDigits + 3; x++)
				obj.texture[0][x] = Engine::SuperChar('-', notRGBA);
			
			obj.texture[1].resize(text.length() + maxDigits + 4);
			obj.texture[1][0] = Engine::SuperChar('|', notRGBA);
			obj.texture[1][1] = Engine::SuperChar(' ', notRGBA);
			obj.texture[1][text.length() + maxDigits + 2] = Engine::SuperChar(' ');
			obj.texture[1][text.length() + maxDigits + 3] = Engine::SuperChar('|');
			for (size_t x = 2; x < text.length() + 2; x++)
				obj.texture[1][x] = Engine::SuperChar(text[x - 2], notRGBA);
			
			int defaultNumInt = 0;
			for (size_t i = 0; i < defaultNum.length() && i < maxDigits; i++)
			{
				defaultNumInt *= 10;
				defaultNumInt += defaultNum[i] - '0';
			}
			for (size_t x = 0; x < defaultNum.length() && x < maxDigits; x++)
			{
				obj.texture[1][2 + text.length() + x] = Engine::SuperChar(defaultNum[x], notRGBA);
				currentDigit++;
			}
			number = defaultNumInt;

			obj.texture[2].resize(text.length() + maxDigits + 4);
			obj.texture[2][0] = Engine::SuperChar('#', notRGBA);
			obj.texture[2][text.length() + maxDigits + 3] = Engine::SuperChar('#', notRGBA);
			for (size_t x = 1; x < text.length() + maxDigits + 3; x++)
				obj.texture[2][x] = Engine::SuperChar('-', notRGBA);
			// Collider
			obj.collider.resize(3, std::vector<bool>(text.length() + 4));
			obj.colliderActive = false;
			for (size_t y = 0; y < obj.collider.size(); y++)
			{
				for (size_t x = 0; x < obj.collider[y].size(); x++)
					obj.collider[y][x] = true;
			}
			// Adding object to layer
			layer->AddObject(&obj);
		}
	};

	class StringInputField
	{
	public:
		std::string string;

		bool selected = false;

		std::function<void()> OnInsert;

		Engine::RGBA notRGBA, selectedRGBA;

		Engine::Object obj;

	private:
		std::string text;
		int currentChar = 0;
		int maxChars;
		
		void InternalInsert()
		{
			if (selected)
			{
				// Remove character
				if (Engine::lastDownVirtualKey == VK_BACK || Engine::lastDownVirtualKey == VK_DELETE)
				{
					if (currentChar == 0)
						return;

					obj.texture[1][1 + text.length() + currentChar].character = ' ';
					string.resize(string.size() - 1);
					currentChar--;
				}
				else if(Engine::lastDownKeyRequiresShift)
				{
					if (currentChar == maxChars)
						return;
					
					IsKeyDown(VK_SHIFT);
					if (IsKeyDown(VK_SHIFT))
					{
						obj.texture[1][2 + text.length() + currentChar].character = Engine::lastDownKey;
						string.push_back(Engine::lastDownKey);
						currentChar++;
					}
				}
				else
				{
					if (currentChar == maxChars)
						return;
					IsKeyDown(VK_SHIFT);
					if (!IsKeyDown(VK_SHIFT))
					{
						obj.texture[1][2 + text.length() + currentChar].character = Engine::lastDownKey;
						string.push_back(Engine::lastDownKey);
						currentChar++;
					}
				}
				if (OnInsert)
					OnInsert();
			}
		}

	public:

		void Select()
		{
			if (selected == false)
			{
				for (size_t y = 0; y < obj.texture.size(); y++)
				{
					for (size_t x = 0; x < obj.texture[y].size(); x++)
						obj.texture[y][x].frgba = selectedRGBA;
				}
				selected = true;
			}
		}

		void Deselect()
		{
			if (selected == true)
			{
				for (size_t y = 0; y < obj.texture.size(); y++)
				{
					for (size_t x = 0; x < obj.texture[y].size(); x++)
						obj.texture[y][x].frgba = notRGBA;
				}
				selected = false;
			}
		}

		StringInputField(Engine::Layer* layer = 0, std::function<void()> OnInsert = 0, std::vector<int> allowedCharacters = {}, Engine::Vector2D pos = { 0, 0 }, std::string text = "Value = ",
			unsigned int maxChars = 8, std::string defaultString = "String", Engine::RGBA notRGBA = { 150, 150, 150, 1.0f }, Engine::RGBA selectedRGBA = { 255, 255, 255, 1.0f })
			: OnInsert(OnInsert), maxChars(maxChars), notRGBA(notRGBA), selectedRGBA(selectedRGBA), text(text), string(defaultString)
		{
			// Registering input
			for (size_t i = 0; i < allowedCharacters.size(); i++)
				Engine::RegisterInputHandler(allowedCharacters[i], true, std::bind(&StringInputField::InternalInsert, this));

			Engine::RegisterInputHandler(VK_DELETE, true, std::bind(&StringInputField::InternalInsert, this), true);
			Engine::RegisterInputHandler(VK_BACK, true, std::bind(&StringInputField::InternalInsert, this), true);

			// *** Creating the object ***
			obj.pos = pos;
			// Texture
			obj.texture.resize(3);
			obj.texture[0].resize(text.length() + maxChars + 4);
			obj.texture[0][0] = Engine::SuperChar('#', notRGBA);
			obj.texture[0][text.length() + maxChars + 3] = Engine::SuperChar('#', notRGBA);
			for (size_t x = 1; x < text.length() + maxChars + 3; x++)
				obj.texture[0][x] = Engine::SuperChar('-', notRGBA);
			obj.texture[1].resize(text.length() + maxChars + 4);
			obj.texture[1][0] = Engine::SuperChar('|', notRGBA);
			obj.texture[1][1] = Engine::SuperChar(' ', notRGBA);
			obj.texture[1][text.length() + maxChars + 2] = Engine::SuperChar(' ');
			obj.texture[1][text.length() + maxChars + 3] = Engine::SuperChar('|');
			for (size_t x = 2; x < text.length() + 2; x++)
				obj.texture[1][x] = Engine::SuperChar(text[x - 2], notRGBA);
			for (size_t x = 0; x < maxChars && x < defaultString.size(); x++)
			{
				obj.texture[1][2 + text.length() + x] = Engine::SuperChar(defaultString[x], notRGBA);
				currentChar++;
			}
			obj.texture[2].resize(text.length() + maxChars + 4);
			obj.texture[2][0] = Engine::SuperChar('#', notRGBA);
			obj.texture[2][text.length() + maxChars + 3] = Engine::SuperChar('#', notRGBA);
			for (size_t x = 1; x < text.length() + maxChars + 3; x++)
				obj.texture[2][x] = Engine::SuperChar('-', notRGBA);
			// Collider
			obj.collider.resize(3, std::vector<bool>(text.length() + 4));
			obj.colliderActive = false;
			for (size_t y = 0; y < obj.collider.size(); y++)
			{
				for (size_t x = 0; x < obj.collider[y].size(); x++)
					obj.collider[y][x] = true;
			}
			// Adding object to layer
			if (layer)
				layer->AddObject(&obj);
		}
	};
}