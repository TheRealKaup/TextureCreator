#include "engine.hpp"

#define maxInputLength 7

std::vector<Engine::Input::Handler> Engine::Input::handlers = {};
size_t Engine::Input::handlerIndex = -1;

std::string Engine::Input::input(maxInputLength, '\0');

void Engine::Input::Handler::AddCall(std::function<void()> call, bool onTick)
{
	onTicks.push_back(onTick);
	calls.push_back(call);
}
Engine::Input::Handler::Handler(const std::string& input, std::function<void()> call, bool onTick) : input(input)
{
	onTicks.push_back(onTick);
	calls.push_back(call);
}

void Engine::Input::RegisterHandler(const std::string& input, std::function<void()> call, bool onTick)
{
	// If a handler already exists for this input, add the call to the calls vector
	for (size_t i = 0; i < handlers.size(); i++)
		if (handlers[i].input == input)
		{
			handlers[i].AddCall(call, onTick);
			return;
		}
	handlers.push_back(Handler(input, call, onTick));
}
void Engine::Input::RegisterHandler(char input, std::function<void()> call, bool onTick)
{
	// If a handler already exists for this input, add the call to the calls vector
	for (size_t i = 0; i < handlers.size(); i++)
		if (handlers[i].input.length() == 1 && handlers[i].input[0] == input)
		{
			handlers[i].AddCall(call, onTick);
			return;
		}
	handlers.push_back(Handler({input}, call, onTick));
}

uint32_t Engine::Input::Call()
{
	uint32_t counter = 0;
	for (size_t i = 0; i < handlers.size(); i++)
	{
		if (handlers[i].timesPressed > 0)
		{
			for (size_t j = 0; j < handlers[i].calls.size(); j++)
			{
				if (handlers[i].onTicks[j] && handlers[i].calls[j])
				{
					counter++;
					input = handlers[i].input;
					handlers[i].calls[j]();
				}
			}
			handlers[i].timesPressed = 0;
		}
	}
	return counter;
}

std::string Engine::Input::Get()
{
	static char* buf = new char[maxInputLength];

	// Clear previous buffer
	memset(buf, 0, maxInputLength);

	// Read to buffer (blocking)
	read(0, buf, maxInputLength);

	input.assign(buf);

	// Call handlers
	for (size_t i = 0; i < handlers.size(); i++)
	{
		if (input == handlers[i].input) // If the strings are equal
		{
			handlers[i].timesPressed++;
			for (size_t j = 0; j < handlers[i].calls.size(); j++) // Call the calls
				if (!handlers[i].onTicks[j] && handlers[i].calls[j]) // Check if the call shouldn't be called on tick and isn't null
					handlers[i].calls[j](); // Call
			break;
		}
	}
	
	return input;
}

void Engine::Input::Loop()
{
	while (Engine::running)
		Get();
}

bool Engine::Input::Is(const std::string& stringKey)
{
	return (input == stringKey);
}

bool Engine::Input::Is(char charKey)
{
	return input.length() == 1 && input[0] == charKey;
}

bool Engine::Input::IsNum()
{
	return (input[0] >= '0') && (input[0] <= '9') && (input.length() == 1);
}

unsigned char Engine::Input::Num()
{
	return input[0] - '0';
}

bool Engine::Input::Bigger(char charKey)
{
	return (input[0] >= charKey) && (input[1] == 0);
}

bool Engine::Input::Smaller(char charKey)
{
	return (input[0] <= charKey) && (input[1] == 0);
}

bool Engine::Input::Between(char charKey1, char charKey2)
{
	return (input[0] >= charKey1) && (input[0] <= charKey2) && (input[1] == 0);
}