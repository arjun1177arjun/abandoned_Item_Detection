
#ifndef __speech_onboard__
#define __speech_onboard__


struct ISpVoice; // fwd ref

namespace Speech
{
	class Voice
	{
		ISpVoice * spVoice;

	public:

		Voice();
		~Voice();


		int speak(const char * txt) const;

		// Supported values range from -10 to 10 
		int setRate(int s);

		// Supported values range from 0 to 100 
		int setVolume(int s);
	};
};


#endif // __speech_onboard__


