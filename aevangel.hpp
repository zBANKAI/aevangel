#define AEVANGEL_HPP  // Definição da guarda de inclusão
#define AEVANGEL_HPP  // Repetição da guarda de inclusão (desnecessário)

#pragma region AEVANGEL_INCLUDES  // Início da seção de inclusões
#include <Windows.h>  // Inclusão das APIs do Windows
#include <functional>  // Inclusão da biblioteca para std::function
#include <intrin.h>  // Inclusão para funções intrínsecas
#include <strsafe.h>  // Inclusão para manipulação segura de strings
#include <msclr/marshal_cppstd.h>  // Inclusão para interoperabilidade entre C++ e .NET
#pragma endregion AEVANGEL_INCLUDES  // Fim da seção de inclusões

#pragma region AEVANGEL_WARNINGS  // Início da seção de desativação de avisos
#pragma warning(disable : 4838)  // Desativa aviso sobre conversões de tipo
#pragma warning(disable : 4309)  // Desativa aviso sobre conversão de tipo de inteiro
#pragma warning(disable : 4312)  // Desativa aviso sobre conversão de ponteiro
#pragma warning(disable : 4311)  // Desativa aviso sobre truncamento de ponteiro
#pragma warning(disable : 4302)  // Desativa aviso sobre conversão de ponteiro em inteiro
#pragma warning(disable : 4715)  // Desativa aviso sobre retorno em função não void
#pragma endregion AEVANGEL_WARNINGS  // Fim da seção de desativação de avisos

#ifndef AEVANGEL_NO_FORCEINLINE  // Verifica se a força de inline não está desativada
#if defined(_MSC_VER)  // Verifica se o compilador é o MSVC
#define AEVANGEL_FORCEINLINE __forceinline  // Define força de inline para MSVC
#elif defined(__GNUC__) && __GNUC__ > 3  // Verifica se o compilador é o GCC
#define AEVANGEL_FORCEINLINE inline __attribute__((__always_inline__))  // Define força de inline para GCC
#else
#define AEVANGEL_FORCEINLINE inline  // Define inline padrão
#endif
#else
#define AEVANGEL_FORCEINLINE inline  // Define inline padrão se a força de inline estiver desativada
#endif

namespace AEVANGEL++  // Define o namespace AEVANGEL++
{	
	namespace typedefs  // Define um namespace para typedefs
	{
		using NtQuerySystemInformationTypedef = NTSTATUS(*)(ULONG, PVOID, ULONG, PULONG);  // Define tipo de função

		typedef struct _SYSTEM_CODEINTEGRITY_INFORMATION  // Estrutura para informações de integridade de código
		{
			ULONG Length;  // Comprimento da estrutura
			ULONG CodeIntegrityOptions;  // Opções de integridade de código
		} SYSTEM_CODEINTEGRITY_INFORMATION, *PSYSTEM_CODEINTEGRITY_INFORMATION;  // Define ponteiro para a estrutura

		typedef enum _SYSTEM_INFORMATION_CLASS  // Enum para classes de informações do sistema
		{
			SystemBasicInformation = 0,  // Informações básicas do sistema
			SystemPerformanceInformation = 2,  // Informações de desempenho
			SystemTimeOfDayInformation = 3,  // Informações de tempo do dia
			SystemProcessInformation = 5,  // Informações de processo
			SystemProcessorPerformanceInformation = 8,  // Informações de desempenho do processador
			SystemInterruptInformation = 23,  // Informações de interrupção
			SystemExceptionInformation = 33,  // Informações de exceção
			SystemRegistryQuotaInformation = 37,  // Informações de cota de registro
			SystemLookasideInformation = 45,  // Informações de lookaside
			SystemCodeIntegrityInformation = 103,  // Informações de integridade de código
			SystemPolicyInformation = 134,  // Informações de política
		} SYSTEM_INFORMATION_CLASS;  // Finaliza a enumeração
	}

	namespace utils  // Define um namespace para utilitários
	{
		AEVANGEL_FORCEINLINE const PEB* get_peb() noexcept  // Função para obter o PEB (Process Environment Block)
		{
			#if defined(_M_X64) || defined(__amd64__)  // Verifica se é arquitetura x64
        		return reinterpret_cast<const PEB*>(__readgsqword(0x60));  // Lê o PEB em x64
			#elif defined(_M_IX86) || defined(__i386__)  // Verifica se é arquitetura x86
        		return reinterpret_cast<const PEB*>(__readfsdword(0x30));  // Lê o PEB em x86
			#elif defined(_M_ARM) || defined(__arm__)  // Verifica se é arquitetura ARM
        		return *reinterpret_cast<const PEB**>(_MoveFromCoprocessor(15, 0, 13, 0, 2) + 0x30);  // Lê o PEB em ARM
			#elif defined(_M_ARM64) || defined(__aarch64__)  // Verifica se é arquitetura ARM64
       	 		return *reinterpret_cast<const PEB**>(__getReg(18) + 0x60);  // Lê o PEB em ARM64
			#elif defined(_M_IA64) || defined(__ia64__)  // Verifica se é arquitetura IA64
        		return *reinterpret_cast<const PEB**>(static_cast<char*>(_rdteb()) + 0x60);  // Lê o PEB em IA64
			#else
				#error Unsupported platform.  // Gera erro se a plataforma não for suportada
			#endif
		}
	}

	namespace security  // Define um namespace para funções de segurança
	{
		std::function<void(void)> ProtectionThread = []()  // Função que implementa uma thread de proteção
		{
			while (true)  // Loop infinito
			{
				BYTE* overflow = reinterpret_cast<BYTE*>("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");  // Cria um buffer
				PostQuitMessage(0);  // Envia mensagem para sair
				TerminateProcess(GetCurrentProcess(), 0);  // Termina o processo atual
				PostQuitMessage(0);  // Envia mensagem novamente
				WriteProcessMemory(GetCurrentProcess(), main, overflow, 1024, nullptr);  // Tenta sobrescrever a memória
				WriteProcessMemory(GetCurrentProcess(), FindWindowA, overflow, 1024, nullptr);  // Tenta sobrescrever a função FindWindowA
				WriteProcessMemory(GetCurrentProcess(), memcpy, overflow, 1024, nullptr);  // Tenta sobrescrever a função memcpy
				WriteProcessMemory(GetCurrentProcess(), OpenProcess, overflow, 1024, nullptr);  // Tenta sobrescrever a função OpenProcess
				WriteProcessMemory(GetCurrentProcess(), GetProcAddress, overflow, 1024, nullptr);  // Tenta sobrescrever a função GetProcAddress
				WriteProcessMemory(GetCurrentProcess(), WriteProcessMemory, overflow, 1024, nullptr);  // Tenta sobrescrever a função WriteProcessMemory
				WriteProcessMemory(GetCurrentProcess(), GetAsyncKeyState, overflow, 1024, nullptr);  // Tenta sobrescrever a função GetAsyncKeyState
			}
		};
	
		std::function<void(void)> find_window = []()  // Função que procura janelas de depuração
		{
			if (FindWindowA(nullptr, "IDA v7.0.170914") ||  // Verifica se a janela IDA está aberta
				FindWindowA(nullptr, "x64dbg") ||  // Verifica se a janela x64dbg está aberta
				FindWindowA(nullptr, "Scylla x64 v0.9.8") ||  // Verifica se a janela Scylla está aberta
				FindWindowA(nullptr, "IAT Autosearch"))  // Verifica se a janela IAT Autosearch está aberta
			{
				ProtectionThread();  // Se qualquer janela de depuração for encontrada, ativa a thread de proteção
			}
		};

		std::function<void(void)> is_debugger_present = []()  // Função que verifica se um depurador está presente
		{
			auto is_dbg_present = FALSE;  // Inicializa a variável para indicar presença de depurador
			if (sDebuggerPresent())  // Verifica se a função sDebuggerPresent indica depurador
			{
				ProtectionThread();  // Ativa a thread de proteção
			}
			if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &is_dbg_present))  // Verifica se um depurador remoto está presente
			{
				if (is_dbg_present)  // Se um depurador remoto foi detectado
				{
					ProtectionThread();  // Ativa a thread de proteção
				}
			}
		};

		std::function<void(void)> anti_attach = []()  // Função que impede a anexação de depuradores
		{
			HMODULE h_ntdll = GetModuleHandleA("ntdll.dll");  // Obtém o manipulador do módulo ntdll.dll
			if (!h_ntdll)  // Se não conseguiu obter o manipulador
				return;  // Retorna

			FARPROC p_dbg_break
