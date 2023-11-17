#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <ostream>

class AudioSystem
{
private:
	FMOD::System* system = nullptr;
public:
	AudioSystem()=default;
	~AudioSystem() = default;

	bool Initialize()
	{
		FMOD_RESULT result = FMOD::System_Create(&system); // Create the main system object.
		if (result != FMOD_OK) {
			std::cerr << "FMOD System creation failed: " << FMOD_ErrorString(result) << std::endl;
			return false;
		}

		result = system->init(512, FMOD_INIT_NORMAL, nullptr); // Initialize FMOD.
		if (result != FMOD_OK) {
			std::cerr << "FMOD System initialization failed: " << FMOD_ErrorString(result) << std::endl;
			return false;
		}

		return true;
	};
	void PlaySound( std::string& soundPath) {
		FMOD::Sound* sound = nullptr;
		std::string filepath = "Audio/" + soundPath;
		FMOD_RESULT result = system->createSound(filepath.c_str(), FMOD_DEFAULT, nullptr, &sound);
		if (result != FMOD_OK) {
			std::cerr << "Failed to create sound: " << FMOD_ErrorString(result) << std::endl;
			return;
		}

		result = system->playSound(sound, nullptr, false, nullptr);
		if (result != FMOD_OK) {
			std::cerr << "Failed to play sound: " << FMOD_ErrorString(result) << std::endl;
			if (sound) {
				sound->release(); // Release the sound object if it fails to play.
			}
			return;
		}

		// Note: Normally you would not release the sound immediately after playing it,
		// as it would stop the sound. You would store the sound and channel pointers
		// and release them at the appropriate time (e.g., after the sound has finished playing).
	}
	void Update()
	{
		system->update();
		
	};
	void CleanUp()
	{
		if (system) {
			system->close();
			system->release();
		}
	};

	FMOD::System* GetSystem() const { return system; }
};
