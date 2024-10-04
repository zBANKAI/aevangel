# aevangel++
AEVANGEL++ is a class containing Anti-RE, Anti-Debug and Anti-Hook methods. Easy to use and easy to implement. It is made to work precisely on C++/CLI runtime.


Estrutura

1. Inclusão de headers e desativação de warnings: A primeira parte do código inclui várias bibliotecas padrão e desativa algumas advertências específicas do compilador. Isso sugere que o projeto pode usar código que potencialmente causa avisos, mas esses são intencionalmente ignorados para evitar ruídos desnecessários.


2. Definição de macros: As macros AEVANGEL_FORCEINLINE garantem a inlining de funções, dependendo do compilador, melhorando o desempenho em certas situações.


3. Namespace AEVANGEL++:

typedefs: Define tipos específicos relacionados ao sistema, como NtQuerySystemInformationTypedef, e uma estrutura e enumeração para tratar de informações de integridade do código e outras informações do sistema.

utils: Implementa uma função get_peb que busca o bloco de ambiente do processo (PEB), uma estrutura crucial para acessar várias informações internas do processo.

security: Contém várias funções lambda que implementam técnicas de proteção contra depuradores e sandboxing, como:

ProtectionThread: Realiza um "buffer overflow" intencional e tenta sobrescrever certas funções críticas do processo.

Detecção de janelas de ferramentas de análise como IDA, x64dbg e Scylla.

Verificação de depuradores: Usa funções como CheckRemoteDebuggerPresent e sDebuggerPresent para detectar se o processo está sendo depurado.

Alteração de pontos de quebra (breakpoints): Modifica o comportamento de DbgBreakPoint para evitar que depuradores interrompam o fluxo do programa.

Verificação de drivers do kernel e software de sandboxing: Verifica drivers carregados e módulos em memória, alertando se detectar ferramentas comumente usadas em análise reversa.

O código utiliza uma combinação de técnicas modernas de anti-debugging, como manipulação direta de memória e alterações em comportamentos padrão do sistema para evitar a inspeção por depuradores ou sandboxies.

Há um uso pesado de APIs do Windows, sugerindo que o código foi desenvolvido para ser executado exclusivamente em ambientes Windows.

O código busca ativamente manipular o processo caso detecte qualquer tentativa de depuração, indo ao ponto de tentar encerrar o processo se condições suspeitas forem encontradas.


😐 - "Por que o nome é Aevangel?"

É um acrônimo de um nome de uma pessoa muito importante pra mim.
