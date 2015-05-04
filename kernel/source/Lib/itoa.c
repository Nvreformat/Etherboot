#include <Etherboot/itoa.h>

static char buf[12];

char* ltoa(int64_t Value)
{
	char *p;
	int flg = 0;
	
	if( Value < 0 ) { flg++; Value = -Value; }
	
	p = ultoa(Value);
	if(flg) *--p = '-';
	return p;
}

char* ultoa(uint64_t Value)
{
	char *p;
		 
	p = buf+sizeof(buf);
	*--p = '\0';
		 
	do
	{
		*--p = '0' + Value%10;
		Value/=10;
	}
	while(Value);
	
	return p;
}

char* itoa(int32_t Value)
{
	char *p;
	int flg = 0;
	
	if( Value < 0 ) { flg++; Value = -Value; }
	
	p = uitoa(Value);
	if(flg) *--p = '-';
	return p;
}

char* btoa(uint8_t Value)
{
	char *p;
		 
	p = buf+sizeof(buf);
	*--p = '\0';
		 
	do
	{
		*--p = '0' + Value%10;
		Value/=10;
	}
	while(Value);
	
	return p;
}

char* uitoa(uint32_t Value)
{
	char *p;
		 
	p = buf+sizeof(buf);
	*--p = '\0';
		 
	do
	{
		*--p = '0' + Value%10;
		Value/=10;
	}
	while(Value);
	
	return p;
}
