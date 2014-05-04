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

#define _esp 0
#define _eip 4
/* ------ */
#define _rsp 0
#define _rip 8
#define _rbx 16
#define _rdi 24
#define _rsi 32

.globl coctx_swap 
.type  coctx_swap, @function
coctx_swap:

#if defined(__i386__)

	leal -4(%esp),%esp
	pushl %ebx
	pushl %ecx
	pushl %edx

	pushl %edi
	pushl %esi
	pushl %ebp
	leal 28(%esp),%esp

	/* sp,ip */
	/* param old ctx */
	movl 4(%esp), %eax

	movl (%esp), %ecx
	movl %ecx, _eip(%eax)

	leal 4(%esp), %ecx
	movl %ecx, _esp(%eax)

	/* param new ctx */
	movl 8(%esp), %eax

	/* push ret ( ip,sp ) */
	movl _eip(%eax), %ecx
	movl _esp(%eax), %esp
	pushl %ecx

	leal -28(%esp),%esp
	popl %ebp
	popl %esi
	popl %edi

	popl %edx
	popl %ecx
	popl %ebx
	leal 4(%esp),%esp
	
	movl $0, %eax
	ret

#elif defined(__x86_64__)

	leaq -8(%rsp),%rsp
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rdx
	pushq %rcx	
	pushq %r8
	pushq %r9
	leaq 80(%rsp),%rsp	

	movq %rbx,_rbx(%rdi)
	movq %rdi,_rdi(%rdi)
	movq %rsi,_rsi(%rdi)
	/* sp */
	movq (%rsp), %rcx
	movq %rcx, _rip(%rdi)
	leaq 8(%rsp), %rcx		
	movq %rcx, _rsp(%rdi)

	/* sp */
	movq _rip(%rsi), %rcx
	movq _rsp(%rsi), %rsp
	pushq %rcx

	movq _rbx(%rsi),%rbx
	movq _rdi(%rsi),%rdi
	movq _rsi(%rsi),%rsi

	leaq -80(%rsp),%rsp
	popq %r9
	popq %r8
	popq %rcx	
	popq %rdx
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	leaq 8(%rsp),%rsp	

	xorl %eax, %eax
	ret
#endif
