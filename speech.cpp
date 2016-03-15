
#include <windows.h>
#include <sapi.h>
#include "speech.h"


#define COM_RELEASE(x) { if ((x)) (x)->Release(); (x) = NULL; }


namespace Speech
{
	struct _ComUser
	{
		_ComUser()	{ CoInitialize(0); }
		~_ComUser() { CoUninitialize(); }
	} _we_need_a_singleton_per_module;


	inline int w2a(WCHAR *in, char *out)
	{
		out[0] = 0;
		return WideCharToMultiByte(CP_ACP, 0, in, -1, out, MAX_PATH, 0, 0);
	}

	inline int a2w(const char *in, WCHAR *out)
	{
		out[0] = 0;
		return MultiByteToWideChar(CP_ACP, 0, in, -1, out, MAX_PATH);
	}




	Voice::Voice()
		: spVoice(0)
	{
		HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (LPVOID *)&(spVoice));
	}


	Voice::~Voice()
	{
		COM_RELEASE(spVoice);
	}



	int Voice::speak(const char * txt) const
	{
		if (!spVoice)
			return 0;

		WCHAR wtxt[800];
		a2w(txt, wtxt);

		ULONG pulstream = 0;
		HRESULT hr = spVoice->Speak(wtxt, 0, &pulstream);

		return hr == S_OK;
	}


	// Supported values range from -10 to 10 
	int Voice::setRate(int s)
	{
		if (!spVoice)
			return 0;

		HRESULT hr = spVoice->SetRate(s);

		return hr == S_OK;
	}

	// Supported values range from 0 to 100 
	int Voice::setVolume(int s)
	{
		if (!spVoice)
			return 0;

		HRESULT hr = spVoice->SetVolume(s);

		return hr == S_OK;
	}
}


