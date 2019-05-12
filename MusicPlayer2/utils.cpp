#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool utils::bytesToSize(double bytes, char * bytestring, int bufSize)
{
	if (bytes <= 0)
		return false;

	float tb = 1099511627776.0f;
	float gb = 1073741824.0f;
	float mb = 1048576.0f;
	float kb = 1024.0f;

	char * returnSize = (char *)malloc(bufSize);
	if (returnSize == NULL)
		return false;

	if (bytes >= tb)
		sprintf(returnSize, "%.2lfTB", (double)bytes / tb);
	else if (bytes >= gb && bytes < tb)
		sprintf(returnSize, "%.2lfGB", (double)bytes / gb);
	else if (bytes >= mb && bytes < gb)
		sprintf(returnSize, "%.2lfMB", (double)bytes / mb);
	else if (bytes >= kb && bytes < mb)
		sprintf(returnSize, "%.2lfKB", (double)bytes / kb);
	else if (bytes < kb)
		sprintf(returnSize, "%.2lfbytes", bytes);
	else
		sprintf(returnSize, "%.2lfbytes", bytes);

	memcpy((void *)bytestring, (void *)returnSize, bufSize);
	free(returnSize);
	return true;
}
