# aevangel++

<div align="center">
<img src="https://github.com/zBANKAI/aevangel/blob/39eaeb44dd2eba0e2f45ced6030bea5a34e41d15/aevangel.jpeg" width="600px" />
</div>



AEVANGEL++ é um "class containing" que contém métodos Anti-RE, Anti-Debug e Anti-Hook. Fácil de usar e fácil de implementar. Feito para funcionar precisamente em tempo de execução C++/CLI.


--> Estrutura do Código


Inclusão de Headers e Desativação de Warnings: A primeira parte do código importa várias bibliotecas padrão e desativa algumas advertências do compilador. Isso indica que o projeto pode estar utilizando código que gera avisos, mas esses são ignorados intencionalmente para evitar ruídos desnecessários.

Definição de Macros: A macro AEVANGEL_FORCEINLINE assegura a inlining de funções, dependendo do compilador, o que pode melhorar o desempenho em determinadas situações.

Namespace AEVANGEL++:

typedefs: Define tipos específicos que estão relacionados ao sistema, como NtQuerySystemInformationTypedef, além de uma estrutura e enumeração para lidar com informações de integridade do código e outras informações do sistema.

utils: Implementa a função get_peb, que busca o bloco de ambiente do processo (PEB), uma estrutura essencial para acessar diversas informações internas do processo.

security: Contém várias funções lambda que implementam técnicas de proteção contra depuradores e sandboxing, como:

ProtectionThread: Executa um "buffer overflow" intencional e tenta sobrescrever funções críticas do processo.

Detecção de janelas de ferramentas de análise como IDA, x64dbg e Scylla.

Verificação de depuradores: Utiliza funções como CheckRemoteDebuggerPresent e sDebuggerPresent para identificar se o processo está sendo depurado.

Alteração de pontos de quebra (breakpoints): Modifica o comportamento de DbgBreakPoint para impedir que depuradores interrompam o fluxo do programa.

Verificação de drivers do kernel e software de sandboxing: Analisa drivers carregados e módulos em memória, alertando sobre ferramentas comumente utilizadas em análise reversa.


O código utiliza uma combinação de técnicas modernas de anti-debugging, manipulando diretamente a memória e alterando comportamentos padrão do sistema para evitar a inspeção por depuradores ou sandboxing.

Além disso, há um uso intenso de APIs do Windows, o código foi desenvolvido exclusivamente para ambientes Windows. Ele busca ativamente manipular o processo se detectar qualquer tentativa de depuração, chegando a encerrar o processo caso condições suspeitas sejam encontradas.
