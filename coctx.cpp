/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/

#include "coctx.h"
#include <string.h>


#define ESP 0
#define EIP 1
// -----------
#define RSP 0
#define RIP 1
#define RBX 2
#define RDI 3
#define RSI 4

extern "C"
{
	extern void coctx_swap( coctx_t *,coctx_t* ) asm("coctx_swap");
};
#if defined(__i386__)
int coctx_init( coctx_t *ctx )
{
	memset( ctx,0,sizeof(*ctx));
	return 0;
}
int coctx_make( coctx_t *ctx,coctx_pfn_t pfn,const void *s,const void *s1 )
{

	char *sp = ctx->ss_sp + ctx->ss_size ;
	sp = (char*)((unsigned long)sp & -16L); 

	int len = sizeof(coctx_param_t) + 64;
	memset( sp - len,0,len );
	ctx->routine = pfn;
	ctx->s1 = s;
	ctx->s2 = s1;

	ctx->param = (coctx_param_t*)sp ;
	ctx->param->f = pfn;	
	ctx->param->f_link = 0;
	ctx->param->s1 = s;
	ctx->param->s2 = s1;

	ctx->regs[ ESP ] = (char*)(ctx->param) + sizeof(void*);
	ctx->regs[ EIP ] = (char*)pfn;


	return 0;
}
#elif defined(__x86_64__)
int coctx_make( coctx_t *ctx,coctx_pfn_t pfn,const void *s,const void *s1 )
{
	char *stack = ctx->ss_sp;
	*stack = 0;

	char *sp = stack + ctx->ss_size - 1;
	sp = (char*)( ( (unsigned long)sp & -16LL ) - 8);

	int len = sizeof(coctx_param_t) + 64;
	memset( sp - len,0,len );

	ctx->routine = pfn;
	ctx->s1 = s;
	ctx->s2 = s1;

	ctx->param = (coctx_param_t*)sp;
	ctx->param->f = pfn;	
	ctx->param->f_link = 0;
	ctx->param->s1 = s;
	ctx->param->s2 = s1;

	ctx->regs[ RBX ] = stack + ctx->ss_size - 1;
	ctx->regs[ RSP ] = (char*)(ctx->param) + 8;
	ctx->regs[ RIP ] = (char*)pfn;

	ctx->regs[ RDI ] = (char*)s;
	ctx->regs[ RSI ] = (char*)s1;

	return 0;
}
int coctx_init( coctx_t *ctx )
{
	memset( ctx,0,sizeof(*ctx));
	return 0;
}

#endif

