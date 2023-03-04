#pragma once
#include "Engine.h"

#define Characters_Lower 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
#define Characters_Upper 'Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M'
#define Characters_Digits '1','2','3','4','5','6','7','8','9','0'
#define Characters_Special '`','~','!','@','#','$','%','^','&','*','(',')','-','_','=','+','[','{',']','}','\\','|',';',':','\'','"',',','<','.','>','/','?'
#define Characters_All Characters_Lower, Characters_Upper, Characters_Digits, Characters_Special

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
				down = true;
				if (OnDown)
					OnDown();

				for (size_t i = 0; i < obj.textures.size(); i++)
				{
					for (size_t y = 0; y < obj.textures[i].t.size(); y++)
					{
						for (size_t x = 0; x < obj.textures[i].t[y].size(); x++)
							obj.textures[i].t[y][x].frgba = downRGBA;
					}
				}
			}
		}

		void InsideOnUp()
		{
			if (selected)
			{
				down = false;
				if (OnUp)
					OnUp();

				for (size_t i = 0; i < obj.textures.size(); i++)
				{
					for (size_t y = 0; y < obj.textures[i].t.size(); y++)
					{
						for (size_t x = 0; x < obj.textures[i].t[y].size(); x++)
							obj.textures[i].t[y][x].frgba = selectedRGBA;
					}
				}
			}
		}

	public:
		void Select()
		{
			for (size_t i = 0; i < obj.textures.size(); i++)
			{
				for (size_t y = 0; y < obj.textures[i].t.size(); y++)
				{
					for (size_t x = 0; x < obj.textures[i].t[y].size(); x++)
						obj.textures[i].t[y][x].frgba = selectedRGBA;
				}
			}

			selected = true;
		}  

		void Deselect()
		{
			for (size_t i = 0; i < obj.textures.size(); i++)
			{
				for (size_t y = 0; y < obj.textures[i].t.size(); y++)
				{
					for (size_t x = 0; x < obj.textures[i].t[y].size(); x++)
						obj.textures[i].t[y][x].frgba = notRGBA;
				}
			}

			selected = false;
		}

		Button(Engine::Layer* layer, std::function<void()> OnDown, std::function<void()> OnUp, int key = VK_RETURN, Engine::Vector2D pos = { 0, 0 }, std::string text = "Button",
			Engine::RGBA notRGBA = { 150, 150, 150, 1.0f }, Engine::RGBA selectedRGBA = { 255, 255, 255, 1.0f }, Engine::RGBA downRGBA = { 150, 150, 255, 1.0f })
			: OnUp(OnUp), OnDown(OnDown), notRGBA(notRGBA), selectedRGBA(selectedRGBA), downRGBA(downRGBA)
		{
			Engine::RegisterInputHandler(key, true, std::bind(&Button::InsideOnDown, this));
			Engine::RegisterInputHandler(key, false, std::bind(&Button::InsideOnUp, this));
			
			obj.pos = pos;

			// Texture
			obj.textures.resize(2);
			
			obj.textures[0].Write({ text }, notRGBA, { 0, 0, 0, 0.0f }, { 1, 1 });
			
			obj.textures[1].Block({ 2 + text.size(), 3 }, Engine::SuperChar(' ', {0, 0, 0, 0.0f}), {0U, 0U});
			obj.textures[1].t[0][0] = Engine::SuperChar('#', notRGBA);
			obj.textures[1].t[0][text.length() + 1] = Engine::SuperChar('#', notRGBA);
			for (size_t x = 1; x <= text.length(); x++)
				obj.textures[1].t[0][x] = Engine::SuperChar('-', notRGBA);
			obj.textures[1].t[1][0] = Engine::SuperChar('|', notRGBA);
			obj.textures[1].t[1][text.length() + 1] = Engine::SuperChar('|', notRGBA);
			obj.textures[1].t[2][0] = Engine::SuperChar('#', notRGBA);
			obj.textures[1].t[2][text.length() + 1] = Engine::SuperChar('#', notRGBA);
			for (size_t x = 1; x <= text.length(); x++)
				obj.textures[1].t[2][x] = Engine::SuperChar('-', notRGBA);

			layer->AddObject(&obj);
		}
	};

	class IntInputField
	{
	public:
		int number = 0;
		int visibleNumber = 0;

		bool selected = false;

		int currentDigit = 0;
		
		std::function<void()> OnInsert;

		Engine::RGBA notRGBA, selectedRGBA;

		Engine::Object obj;

	private:
		unsigned int min, max;
		unsigned char maxDigits = 0;
		unsigned char minDigits = 0;
		
		void InternalInsert()
		{
			if (selected)
			{
				if (Engine::lastDownVirtualKey == VK_BACK || Engine::lastDownVirtualKey == VK_DELETE)
				{
					if (currentDigit == 0)
						return;

					visibleNumber /= 10;

					currentDigit--;
					obj.textures[0].t[0][currentDigit].character = ' ';
				}
				else if (Engine::lastDownKey >= '0' || Engine::lastDownKey <= '9')
				{
					if (currentDigit == maxDigits)
						return;

					obj.textures[0].t[0][currentDigit].character = Engine::lastDownKey;
					currentDigit++;

					visibleNumber = visibleNumber * 10 + Engine::lastDownKey - '0';
				}

				number = visibleNumber;
				if (number < min)
					number = min;
				if (number > max)
					number = max;

				if (OnInsert)
					OnInsert();
			}
		}

	public:
		void Select()
		{
			for (size_t i = 0; i < obj.textures.size(); i++)
			{
				for (size_t y = 0; y < obj.textures[i].t.size(); y++)
				{
					for (size_t x = 0; x < obj.textures[i].t[y].size(); x++)
						obj.textures[i].t[y][x].frgba = selectedRGBA;
				}
			}
			selected = true;
		}

		void Deselect()
		{
			if (visibleNumber < min)
			{
				visibleNumber = min;

				currentDigit = minDigits;

				std::string newTexture = std::to_string(min);
				newTexture.resize(maxDigits, ' ');
				obj.textures[0].Write({ newTexture }, notRGBA, Engine::RGBA(), obj.textures[0].pos);
			}
			if (visibleNumber > max)
			{
				visibleNumber = max;

				currentDigit = maxDigits;

				std::string newTexture = std::to_string(max);
				newTexture.resize(maxDigits, ' ');
				obj.textures[0].Write({ newTexture }, notRGBA, Engine::RGBA(), obj.textures[0].pos);
			}

			for (size_t i = 0; i < obj.textures.size(); i++)
			{
				for (size_t y = 0; y < obj.textures[i].t.size(); y++)
				{
					for (size_t x = 0; x < obj.textures[i].t[y].size(); x++)
						obj.textures[i].t[y][x].frgba = notRGBA;
				}
			}

			if (visibleNumber >= min && visibleNumber <= max)
				number = visibleNumber;

			selected = false;
		}

		void ChangeValue(std::string newNumber)
		{
			obj.textures[0].Block({ maxDigits, 1 }, Engine::SuperChar(' ', notRGBA, { 0, 0, 0, 0.0f }), { 1 + (int)obj.textures[1].t[0].size(), 1 });
			number = 0;
			currentDigit = 0;
			for (int x = 0; x < maxDigits && x < newNumber.length(); x++)
			{
				currentDigit++;
				number *= 10;
				number += newNumber[x] - '0';
				obj.textures[0].t[0][x] = Engine::SuperChar(newNumber[x], notRGBA);
			}
			visibleNumber = number;
		}

		IntInputField(Engine::Layer* layer, std::function<void()> OnInsert, unsigned int min = 0, unsigned int max = 255, std::string defaultNum = "0", Engine::Vector2D pos = {0, 0},
			std::string text = "Value = ", Engine::RGBA notRGBA = { 150, 150, 150, 1.0f }, Engine::RGBA selectedRGBA = { 255, 255, 255, 1.0f })
			: OnInsert(OnInsert), min(min), max(max), notRGBA(notRGBA), selectedRGBA(selectedRGBA)
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

			obj.pos = pos;

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

			// Texture
			obj.textures.resize(3);

			obj.textures[0].Block({ maxDigits, 1 }, Engine::SuperChar(' ', notRGBA, { 0, 0, 0, 0.0f }), { 1 + (int)text.length(), 1 });
			for (int x = 0; x < maxDigits && x < defaultNum.length(); x++)
			{
				currentDigit++;
				number *= 10;
				number += defaultNum[x] - '0';
				obj.textures[0].t[0][x] = Engine::SuperChar(defaultNum[x], notRGBA);
			}
			visibleNumber = number;

			obj.textures[1].Write({ text }, notRGBA, Engine::RGBA(), { 1, 1 });

			obj.textures[2].Block({ 2 + text.size() + maxDigits, 3 }, Engine::SuperChar(' ', { 0, 0, 0, 0.0f }), { 0, 0 });
			obj.textures[2].t[0][0] = Engine::SuperChar('#', notRGBA);
			obj.textures[2].t[0][obj.textures[2].t[0].size() - 1] = Engine::SuperChar('#', notRGBA);
			for (int x = 1; x < obj.textures[2].t[0].size() - 1; x++)
				obj.textures[2].t[0][x] = Engine::SuperChar('-', notRGBA);
			obj.textures[2].t[1][0] = Engine::SuperChar('|', notRGBA);
			obj.textures[2].t[1][obj.textures[2].t[1].size() - 1] = Engine::SuperChar('|', notRGBA);
			obj.textures[2].t[2][0] = Engine::SuperChar('#', notRGBA);
			obj.textures[2].t[2][obj.textures[2].t[2].size() - 1] = Engine::SuperChar('#', notRGBA);
			for (int x = 1; x < obj.textures[2].t[2].size() - 1; x++)
				obj.textures[2].t[2][x] = Engine::SuperChar('-', notRGBA);

			layer->AddObject(&obj);
		}
	};

	class StringInputField
	{
	public:
		std::string string = "";

		bool selected = false;

		std::function<void()> OnInsert;

		Engine::RGBA notRGBA, selectedRGBA;

		Engine::Object obj;

	private:
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

					currentChar--;
					string.resize(string.size() - 1);

					obj.textures[0].t[0][currentChar].character = ' ';
				}
				else if(Engine::lastDownKeyRequiresShift)
				{
					if (currentChar == maxChars)
						return;
					
					IsKeyDown(VK_SHIFT);
					if (IsKeyDown(VK_SHIFT))
					{
						obj.textures[0].t[0][currentChar].character = Engine::lastDownKey;
						
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
						obj.textures[0].t[0][currentChar].character = Engine::lastDownKey;
						
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
			for (size_t i = 0; i < obj.textures.size(); i++)
			{
				for (size_t y = 0; y < obj.textures[i].t.size(); y++)
				{
					for (size_t x = 0; x < obj.textures[i].t[y].size(); x++)
						obj.textures[i].t[y][x].frgba = selectedRGBA;
				}
			}
			
			selected = true;
		}

		void Deselect()
		{
			for (size_t i = 0; i < obj.textures.size(); i++)
			{
				for (size_t y = 0; y < obj.textures[i].t.size(); y++)
				{
					for (size_t x = 0; x < obj.textures[i].t[y].size(); x++)
						obj.textures[i].t[y][x].frgba = notRGBA;
				}
			}
			selected = false;
		}

		void ChangeValue(std::string newString)
		{
			string.resize(newString.length() > maxChars ? maxChars : newString.length());
			for (size_t x = 0; x < maxChars; x++)
			{
				if (x < string.length())
				{
					string[x] = newString[x];
					obj.textures[0].t[0][x].character = newString[x];
				}
				else
					obj.textures[0].t[0][x].character = ' ';
			}
			currentChar = string.length();
		}

		StringInputField(Engine::Layer* layer = 0, std::function<void()> OnInsert = 0, std::vector<int> allowedCharacters = {}, Engine::Vector2D pos = { 0, 0 }, std::string text = "Value = ",
			unsigned int maxChars = 8, std::string defaultString = "String", Engine::RGBA notRGBA = { 150, 150, 150, 1.0f }, Engine::RGBA selectedRGBA = { 255, 255, 255, 1.0f })
			: OnInsert(OnInsert), maxChars(maxChars), notRGBA(notRGBA), selectedRGBA(selectedRGBA)
		{
			// Registering input
			for (size_t i = 0; i < allowedCharacters.size(); i++)
				Engine::RegisterInputHandler(allowedCharacters[i], true, std::bind(&StringInputField::InternalInsert, this));

			Engine::RegisterInputHandler(VK_DELETE, true, std::bind(&StringInputField::InternalInsert, this), true);
			Engine::RegisterInputHandler(VK_BACK, true, std::bind(&StringInputField::InternalInsert, this), true);

			obj.pos = pos;

			// Texture
			Engine::Texture field;
			field.Block({ maxChars, 1 }, Engine::SuperChar(' ', notRGBA, { 0, 0, 0, 0.0f }), { 1 + (int)text.length(), 1 });
			for (int x = 0; x < maxChars && x < defaultString.length(); x++)
			{
				currentChar++;
				string += defaultString[x];
				field.t[0][x] = Engine::SuperChar(defaultString[x], notRGBA);
			}

			Engine::Texture fieldText;
			fieldText.Write({ text }, notRGBA, Engine::RGBA(), { 1, 1 });

			Engine::Texture frame;
			frame.Block({ 2U + text.size() + maxChars, 3 }, Engine::SuperChar(' ', {0, 0, 0, 0.0f}), {0, 0});
			frame.t[0][0] = Engine::SuperChar('#', notRGBA);
			frame.t[0][frame.t[0].size() - 1] = Engine::SuperChar('#', notRGBA);
			for (int x = 1; x < frame.t[0].size() - 1; x++)
				frame.t[0][x] = Engine::SuperChar('-', notRGBA);
			frame.t[1][0] = Engine::SuperChar('|', notRGBA);
			frame.t[1][frame.t[1].size() - 1] = Engine::SuperChar('|', notRGBA);
			frame.t[2][0] = Engine::SuperChar('#', notRGBA);
			frame.t[2][frame.t[2].size() - 1] = Engine::SuperChar('#', notRGBA);
			for (int x = 1; x < frame.t[2].size() - 1; x++)
				frame.t[2][x] = Engine::SuperChar('-', notRGBA);

			obj.textures.resize(3);
			obj.textures[0] = field;
			obj.textures[1] = fieldText;
			obj.textures[2] = frame;

			layer->AddObject(&obj);
		}
	};
}