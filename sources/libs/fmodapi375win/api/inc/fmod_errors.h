#ifndef _FMOD_ERRORS_H
#define _FMOD_ERRORS_H

static const char* FMOD_ErrorString(int errcode)
{
	const char* result;

	switch (errcode)
	{
		case FMOD_ERR_NONE:				result = "No errors";	break;
		case FMOD_ERR_BUSY:				result = "Cannot call this command after FSOUND_Init.  Call FSOUND_Close first.";	break;
		case FMOD_ERR_UNINITIALIZED:	result = "This command failed because FSOUND_Init was not called";	break;
		case FMOD_ERR_PLAY:				result = "Playing the sound failed.";	break;
		case FMOD_ERR_INIT:				result = "Error initializing output device.";	break;
		case FMOD_ERR_ALLOCATED:		result = "The output device is already in use and cannot be reused.";	break;
		case FMOD_ERR_OUTPUT_FORMAT:	result = "Soundcard does not support the features needed for this soundsystem (16bit stereo output)";	break;
		case FMOD_ERR_COOPERATIVELEVEL:	result = "Error setting cooperative level for hardware.";	break;
		case FMOD_ERR_CREATEBUFFER:		result = "Error creating hardware sound buffer.";	break;
		case FMOD_ERR_FILE_NOTFOUND:	result = "File not found";	break;
		case FMOD_ERR_FILE_FORMAT:		result = "Unknown file format";	break;
		case FMOD_ERR_FILE_BAD:			result = "Error loading file";	break;
		case FMOD_ERR_MEMORY:			result = "Not enough memory ";	break;
		case FMOD_ERR_VERSION:			result = "The version number of this file format is not supported";	break;
		case FMOD_ERR_INVALID_PARAM:	result = "An invalid parameter was passed to this function";	break;
		case FMOD_ERR_NO_EAX:			result = "Tried to use an EAX command on a non EAX enabled channel or output.";	break;
		case FMOD_ERR_CHANNEL_ALLOC:	result = "Failed to allocate a new channel";	break;
		case FMOD_ERR_RECORD:			result = "Recording not supported on this device";	break;
		case FMOD_ERR_MEDIAPLAYER:		result = "Required Mediaplayer codec is not installed";	break;

		default :						result = "Unknown error";	break;
	};

	return result;
}

#endif
