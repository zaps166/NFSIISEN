#include <stdint.h>

__attribute__((stdcall)) int DirectXSetupGetVersion(uint32_t *pdwVersion, uint32_t *pdwRevision)
{
	*pdwVersion = 5;
	*pdwRevision = 0;
	return 0;
}
