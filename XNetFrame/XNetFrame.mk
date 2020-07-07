##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=XNetFrame
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/root/XGame"
ProjectPath            := "/root/XGame/XNetFrame"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=root
Date                   :=03/06/2012
CodeLitePath           :="/root/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=../lib/lib$(ProjectName).so
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             := -g $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=$(LibrarySwitch)pthread $(LibrarySwitch)rt 
LibPath                := "$(LibraryPathSwitch)." 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/AdjustableBuffer$(ObjectSuffix) $(IntermediateDirectory)/AuthCrypt$(ObjectSuffix) $(IntermediateDirectory)/Channel$(ObjectSuffix) $(IntermediateDirectory)/BitStream$(ObjectSuffix) $(IntermediateDirectory)/ConsoleViewer$(ObjectSuffix) $(IntermediateDirectory)/Context$(ObjectSuffix) $(IntermediateDirectory)/DispatchService$(ObjectSuffix) $(IntermediateDirectory)/DispatchThread$(ObjectSuffix) $(IntermediateDirectory)/EpollAcceptThread$(ObjectSuffix) \
	$(IntermediateDirectory)/EpollAsyncIoThread$(ObjectSuffix) $(IntermediateDirectory)/EpollService$(ObjectSuffix) $(IntermediateDirectory)/EpollImpl$(ObjectSuffix) $(IntermediateDirectory)/Event$(ObjectSuffix) $(IntermediateDirectory)/XNetSession$(ObjectSuffix) $(IntermediateDirectory)/TcpService$(ObjectSuffix) $(IntermediateDirectory)/Loader$(ObjectSuffix) $(IntermediateDirectory)/Log$(ObjectSuffix) $(IntermediateDirectory)/Exception$(ObjectSuffix) $(IntermediateDirectory)/Thread$(ObjectSuffix) \
	$(IntermediateDirectory)/TcpSocket$(ObjectSuffix) $(IntermediateDirectory)/Monitor$(ObjectSuffix) $(IntermediateDirectory)/MessageHandler$(ObjectSuffix) $(IntermediateDirectory)/Viewer$(ObjectSuffix) $(IntermediateDirectory)/Properties$(ObjectSuffix) $(IntermediateDirectory)/TcpThread$(ObjectSuffix) $(IntermediateDirectory)/ResourceInterface$(ObjectSuffix) $(IntermediateDirectory)/EventHandlerFactory$(ObjectSuffix) $(IntermediateDirectory)/StringTokenizer$(ObjectSuffix) $(IntermediateDirectory)/Stream$(ObjectSuffix) \
	$(IntermediateDirectory)/System$(ObjectSuffix) $(IntermediateDirectory)/Socket$(ObjectSuffix) $(IntermediateDirectory)/RotativeBuffer$(ObjectSuffix) $(IntermediateDirectory)/SocketImpl$(ObjectSuffix) $(IntermediateDirectory)/UdpThread$(ObjectSuffix) $(IntermediateDirectory)/SessionFactory$(ObjectSuffix) $(IntermediateDirectory)/MessageDigest$(ObjectSuffix) $(IntermediateDirectory)/MessageHandlerFactory$(ObjectSuffix) $(IntermediateDirectory)/InetAddress$(ObjectSuffix) $(IntermediateDirectory)/ScheduleService$(ObjectSuffix) \
	$(IntermediateDirectory)/Profiler$(ObjectSuffix) $(IntermediateDirectory)/FileLoader$(ObjectSuffix) $(IntermediateDirectory)/ServerSocket$(ObjectSuffix) $(IntermediateDirectory)/UdpSocket$(ObjectSuffix) $(IntermediateDirectory)/FileViewer$(ObjectSuffix) $(IntermediateDirectory)/ScheduleThread$(ObjectSuffix) $(IntermediateDirectory)/LogThread$(ObjectSuffix) $(IntermediateDirectory)/Session$(ObjectSuffix) $(IntermediateDirectory)/UdpService$(ObjectSuffix) $(IntermediateDirectory)/Random$(ObjectSuffix) \
	$(IntermediateDirectory)/XNetConnector$(ObjectSuffix) $(IntermediateDirectory)/XNetChannel$(ObjectSuffix) $(IntermediateDirectory)/SessionDispatchService$(ObjectSuffix) $(IntermediateDirectory)/ChannelDispatchService$(ObjectSuffix) $(IntermediateDirectory)/ContextDispatchService$(ObjectSuffix) $(IntermediateDirectory)/XNetService$(ObjectSuffix) $(IntermediateDirectory)/XScheduleService$(ObjectSuffix) $(IntermediateDirectory)/TimerEventHandler$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/main.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "/root/XGame/XNetFrame/main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "/root/XGame/XNetFrame/main.cpp"

$(IntermediateDirectory)/AdjustableBuffer$(ObjectSuffix): AdjustableBuffer.cpp $(IntermediateDirectory)/AdjustableBuffer$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/AdjustableBuffer.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/AdjustableBuffer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AdjustableBuffer$(DependSuffix): AdjustableBuffer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/AdjustableBuffer$(ObjectSuffix) -MF$(IntermediateDirectory)/AdjustableBuffer$(DependSuffix) -MM "/root/XGame/XNetFrame/AdjustableBuffer.cpp"

$(IntermediateDirectory)/AdjustableBuffer$(PreprocessSuffix): AdjustableBuffer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AdjustableBuffer$(PreprocessSuffix) "/root/XGame/XNetFrame/AdjustableBuffer.cpp"

$(IntermediateDirectory)/AuthCrypt$(ObjectSuffix): AuthCrypt.cpp $(IntermediateDirectory)/AuthCrypt$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/AuthCrypt.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/AuthCrypt$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AuthCrypt$(DependSuffix): AuthCrypt.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/AuthCrypt$(ObjectSuffix) -MF$(IntermediateDirectory)/AuthCrypt$(DependSuffix) -MM "/root/XGame/XNetFrame/AuthCrypt.cpp"

$(IntermediateDirectory)/AuthCrypt$(PreprocessSuffix): AuthCrypt.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AuthCrypt$(PreprocessSuffix) "/root/XGame/XNetFrame/AuthCrypt.cpp"

$(IntermediateDirectory)/Channel$(ObjectSuffix): Channel.cpp $(IntermediateDirectory)/Channel$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Channel.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Channel$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Channel$(DependSuffix): Channel.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Channel$(ObjectSuffix) -MF$(IntermediateDirectory)/Channel$(DependSuffix) -MM "/root/XGame/XNetFrame/Channel.cpp"

$(IntermediateDirectory)/Channel$(PreprocessSuffix): Channel.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Channel$(PreprocessSuffix) "/root/XGame/XNetFrame/Channel.cpp"

$(IntermediateDirectory)/BitStream$(ObjectSuffix): BitStream.cpp $(IntermediateDirectory)/BitStream$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/BitStream.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/BitStream$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BitStream$(DependSuffix): BitStream.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/BitStream$(ObjectSuffix) -MF$(IntermediateDirectory)/BitStream$(DependSuffix) -MM "/root/XGame/XNetFrame/BitStream.cpp"

$(IntermediateDirectory)/BitStream$(PreprocessSuffix): BitStream.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BitStream$(PreprocessSuffix) "/root/XGame/XNetFrame/BitStream.cpp"

$(IntermediateDirectory)/ConsoleViewer$(ObjectSuffix): ConsoleViewer.cpp $(IntermediateDirectory)/ConsoleViewer$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/ConsoleViewer.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/ConsoleViewer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConsoleViewer$(DependSuffix): ConsoleViewer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/ConsoleViewer$(ObjectSuffix) -MF$(IntermediateDirectory)/ConsoleViewer$(DependSuffix) -MM "/root/XGame/XNetFrame/ConsoleViewer.cpp"

$(IntermediateDirectory)/ConsoleViewer$(PreprocessSuffix): ConsoleViewer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConsoleViewer$(PreprocessSuffix) "/root/XGame/XNetFrame/ConsoleViewer.cpp"

$(IntermediateDirectory)/Context$(ObjectSuffix): Context.cpp $(IntermediateDirectory)/Context$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Context.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Context$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Context$(DependSuffix): Context.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Context$(ObjectSuffix) -MF$(IntermediateDirectory)/Context$(DependSuffix) -MM "/root/XGame/XNetFrame/Context.cpp"

$(IntermediateDirectory)/Context$(PreprocessSuffix): Context.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Context$(PreprocessSuffix) "/root/XGame/XNetFrame/Context.cpp"

$(IntermediateDirectory)/DispatchService$(ObjectSuffix): DispatchService.cpp $(IntermediateDirectory)/DispatchService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/DispatchService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/DispatchService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DispatchService$(DependSuffix): DispatchService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/DispatchService$(ObjectSuffix) -MF$(IntermediateDirectory)/DispatchService$(DependSuffix) -MM "/root/XGame/XNetFrame/DispatchService.cpp"

$(IntermediateDirectory)/DispatchService$(PreprocessSuffix): DispatchService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DispatchService$(PreprocessSuffix) "/root/XGame/XNetFrame/DispatchService.cpp"

$(IntermediateDirectory)/DispatchThread$(ObjectSuffix): DispatchThread.cpp $(IntermediateDirectory)/DispatchThread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/DispatchThread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/DispatchThread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DispatchThread$(DependSuffix): DispatchThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/DispatchThread$(ObjectSuffix) -MF$(IntermediateDirectory)/DispatchThread$(DependSuffix) -MM "/root/XGame/XNetFrame/DispatchThread.cpp"

$(IntermediateDirectory)/DispatchThread$(PreprocessSuffix): DispatchThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DispatchThread$(PreprocessSuffix) "/root/XGame/XNetFrame/DispatchThread.cpp"

$(IntermediateDirectory)/EpollAcceptThread$(ObjectSuffix): EpollAcceptThread.cpp $(IntermediateDirectory)/EpollAcceptThread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/EpollAcceptThread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/EpollAcceptThread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EpollAcceptThread$(DependSuffix): EpollAcceptThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/EpollAcceptThread$(ObjectSuffix) -MF$(IntermediateDirectory)/EpollAcceptThread$(DependSuffix) -MM "/root/XGame/XNetFrame/EpollAcceptThread.cpp"

$(IntermediateDirectory)/EpollAcceptThread$(PreprocessSuffix): EpollAcceptThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EpollAcceptThread$(PreprocessSuffix) "/root/XGame/XNetFrame/EpollAcceptThread.cpp"

$(IntermediateDirectory)/EpollAsyncIoThread$(ObjectSuffix): EpollAsyncIoThread.cpp $(IntermediateDirectory)/EpollAsyncIoThread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/EpollAsyncIoThread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/EpollAsyncIoThread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EpollAsyncIoThread$(DependSuffix): EpollAsyncIoThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/EpollAsyncIoThread$(ObjectSuffix) -MF$(IntermediateDirectory)/EpollAsyncIoThread$(DependSuffix) -MM "/root/XGame/XNetFrame/EpollAsyncIoThread.cpp"

$(IntermediateDirectory)/EpollAsyncIoThread$(PreprocessSuffix): EpollAsyncIoThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EpollAsyncIoThread$(PreprocessSuffix) "/root/XGame/XNetFrame/EpollAsyncIoThread.cpp"

$(IntermediateDirectory)/EpollService$(ObjectSuffix): EpollService.cpp $(IntermediateDirectory)/EpollService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/EpollService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/EpollService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EpollService$(DependSuffix): EpollService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/EpollService$(ObjectSuffix) -MF$(IntermediateDirectory)/EpollService$(DependSuffix) -MM "/root/XGame/XNetFrame/EpollService.cpp"

$(IntermediateDirectory)/EpollService$(PreprocessSuffix): EpollService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EpollService$(PreprocessSuffix) "/root/XGame/XNetFrame/EpollService.cpp"

$(IntermediateDirectory)/EpollImpl$(ObjectSuffix): EpollImpl.cpp $(IntermediateDirectory)/EpollImpl$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/EpollImpl.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/EpollImpl$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EpollImpl$(DependSuffix): EpollImpl.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/EpollImpl$(ObjectSuffix) -MF$(IntermediateDirectory)/EpollImpl$(DependSuffix) -MM "/root/XGame/XNetFrame/EpollImpl.cpp"

$(IntermediateDirectory)/EpollImpl$(PreprocessSuffix): EpollImpl.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EpollImpl$(PreprocessSuffix) "/root/XGame/XNetFrame/EpollImpl.cpp"

$(IntermediateDirectory)/Event$(ObjectSuffix): Event.cpp $(IntermediateDirectory)/Event$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Event.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Event$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Event$(DependSuffix): Event.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Event$(ObjectSuffix) -MF$(IntermediateDirectory)/Event$(DependSuffix) -MM "/root/XGame/XNetFrame/Event.cpp"

$(IntermediateDirectory)/Event$(PreprocessSuffix): Event.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Event$(PreprocessSuffix) "/root/XGame/XNetFrame/Event.cpp"

$(IntermediateDirectory)/XNetSession$(ObjectSuffix): XNetSession.cpp $(IntermediateDirectory)/XNetSession$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/XNetSession.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/XNetSession$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/XNetSession$(DependSuffix): XNetSession.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/XNetSession$(ObjectSuffix) -MF$(IntermediateDirectory)/XNetSession$(DependSuffix) -MM "/root/XGame/XNetFrame/XNetSession.cpp"

$(IntermediateDirectory)/XNetSession$(PreprocessSuffix): XNetSession.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/XNetSession$(PreprocessSuffix) "/root/XGame/XNetFrame/XNetSession.cpp"

$(IntermediateDirectory)/TcpService$(ObjectSuffix): TcpService.cpp $(IntermediateDirectory)/TcpService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/TcpService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/TcpService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TcpService$(DependSuffix): TcpService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/TcpService$(ObjectSuffix) -MF$(IntermediateDirectory)/TcpService$(DependSuffix) -MM "/root/XGame/XNetFrame/TcpService.cpp"

$(IntermediateDirectory)/TcpService$(PreprocessSuffix): TcpService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TcpService$(PreprocessSuffix) "/root/XGame/XNetFrame/TcpService.cpp"

$(IntermediateDirectory)/Loader$(ObjectSuffix): Loader.cpp $(IntermediateDirectory)/Loader$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Loader.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Loader$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Loader$(DependSuffix): Loader.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Loader$(ObjectSuffix) -MF$(IntermediateDirectory)/Loader$(DependSuffix) -MM "/root/XGame/XNetFrame/Loader.cpp"

$(IntermediateDirectory)/Loader$(PreprocessSuffix): Loader.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Loader$(PreprocessSuffix) "/root/XGame/XNetFrame/Loader.cpp"

$(IntermediateDirectory)/Log$(ObjectSuffix): Log.cpp $(IntermediateDirectory)/Log$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Log.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Log$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Log$(DependSuffix): Log.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Log$(ObjectSuffix) -MF$(IntermediateDirectory)/Log$(DependSuffix) -MM "/root/XGame/XNetFrame/Log.cpp"

$(IntermediateDirectory)/Log$(PreprocessSuffix): Log.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Log$(PreprocessSuffix) "/root/XGame/XNetFrame/Log.cpp"

$(IntermediateDirectory)/Exception$(ObjectSuffix): Exception.cpp $(IntermediateDirectory)/Exception$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Exception.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Exception$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Exception$(DependSuffix): Exception.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Exception$(ObjectSuffix) -MF$(IntermediateDirectory)/Exception$(DependSuffix) -MM "/root/XGame/XNetFrame/Exception.cpp"

$(IntermediateDirectory)/Exception$(PreprocessSuffix): Exception.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Exception$(PreprocessSuffix) "/root/XGame/XNetFrame/Exception.cpp"

$(IntermediateDirectory)/Thread$(ObjectSuffix): Thread.cpp $(IntermediateDirectory)/Thread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Thread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Thread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Thread$(DependSuffix): Thread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Thread$(ObjectSuffix) -MF$(IntermediateDirectory)/Thread$(DependSuffix) -MM "/root/XGame/XNetFrame/Thread.cpp"

$(IntermediateDirectory)/Thread$(PreprocessSuffix): Thread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Thread$(PreprocessSuffix) "/root/XGame/XNetFrame/Thread.cpp"

$(IntermediateDirectory)/TcpSocket$(ObjectSuffix): TcpSocket.cpp $(IntermediateDirectory)/TcpSocket$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/TcpSocket.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/TcpSocket$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TcpSocket$(DependSuffix): TcpSocket.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/TcpSocket$(ObjectSuffix) -MF$(IntermediateDirectory)/TcpSocket$(DependSuffix) -MM "/root/XGame/XNetFrame/TcpSocket.cpp"

$(IntermediateDirectory)/TcpSocket$(PreprocessSuffix): TcpSocket.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TcpSocket$(PreprocessSuffix) "/root/XGame/XNetFrame/TcpSocket.cpp"

$(IntermediateDirectory)/Monitor$(ObjectSuffix): Monitor.cpp $(IntermediateDirectory)/Monitor$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Monitor.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Monitor$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Monitor$(DependSuffix): Monitor.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Monitor$(ObjectSuffix) -MF$(IntermediateDirectory)/Monitor$(DependSuffix) -MM "/root/XGame/XNetFrame/Monitor.cpp"

$(IntermediateDirectory)/Monitor$(PreprocessSuffix): Monitor.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Monitor$(PreprocessSuffix) "/root/XGame/XNetFrame/Monitor.cpp"

$(IntermediateDirectory)/MessageHandler$(ObjectSuffix): MessageHandler.cpp $(IntermediateDirectory)/MessageHandler$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/MessageHandler.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/MessageHandler$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MessageHandler$(DependSuffix): MessageHandler.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/MessageHandler$(ObjectSuffix) -MF$(IntermediateDirectory)/MessageHandler$(DependSuffix) -MM "/root/XGame/XNetFrame/MessageHandler.cpp"

$(IntermediateDirectory)/MessageHandler$(PreprocessSuffix): MessageHandler.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MessageHandler$(PreprocessSuffix) "/root/XGame/XNetFrame/MessageHandler.cpp"

$(IntermediateDirectory)/Viewer$(ObjectSuffix): Viewer.cpp $(IntermediateDirectory)/Viewer$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Viewer.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Viewer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Viewer$(DependSuffix): Viewer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Viewer$(ObjectSuffix) -MF$(IntermediateDirectory)/Viewer$(DependSuffix) -MM "/root/XGame/XNetFrame/Viewer.cpp"

$(IntermediateDirectory)/Viewer$(PreprocessSuffix): Viewer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Viewer$(PreprocessSuffix) "/root/XGame/XNetFrame/Viewer.cpp"

$(IntermediateDirectory)/Properties$(ObjectSuffix): Properties.cpp $(IntermediateDirectory)/Properties$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Properties.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Properties$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Properties$(DependSuffix): Properties.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Properties$(ObjectSuffix) -MF$(IntermediateDirectory)/Properties$(DependSuffix) -MM "/root/XGame/XNetFrame/Properties.cpp"

$(IntermediateDirectory)/Properties$(PreprocessSuffix): Properties.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Properties$(PreprocessSuffix) "/root/XGame/XNetFrame/Properties.cpp"

$(IntermediateDirectory)/TcpThread$(ObjectSuffix): TcpThread.cpp $(IntermediateDirectory)/TcpThread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/TcpThread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/TcpThread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TcpThread$(DependSuffix): TcpThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/TcpThread$(ObjectSuffix) -MF$(IntermediateDirectory)/TcpThread$(DependSuffix) -MM "/root/XGame/XNetFrame/TcpThread.cpp"

$(IntermediateDirectory)/TcpThread$(PreprocessSuffix): TcpThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TcpThread$(PreprocessSuffix) "/root/XGame/XNetFrame/TcpThread.cpp"

$(IntermediateDirectory)/ResourceInterface$(ObjectSuffix): ResourceInterface.cpp $(IntermediateDirectory)/ResourceInterface$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/ResourceInterface.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/ResourceInterface$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ResourceInterface$(DependSuffix): ResourceInterface.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/ResourceInterface$(ObjectSuffix) -MF$(IntermediateDirectory)/ResourceInterface$(DependSuffix) -MM "/root/XGame/XNetFrame/ResourceInterface.cpp"

$(IntermediateDirectory)/ResourceInterface$(PreprocessSuffix): ResourceInterface.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ResourceInterface$(PreprocessSuffix) "/root/XGame/XNetFrame/ResourceInterface.cpp"

$(IntermediateDirectory)/EventHandlerFactory$(ObjectSuffix): EventHandlerFactory.cpp $(IntermediateDirectory)/EventHandlerFactory$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/EventHandlerFactory.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/EventHandlerFactory$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/EventHandlerFactory$(DependSuffix): EventHandlerFactory.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/EventHandlerFactory$(ObjectSuffix) -MF$(IntermediateDirectory)/EventHandlerFactory$(DependSuffix) -MM "/root/XGame/XNetFrame/EventHandlerFactory.cpp"

$(IntermediateDirectory)/EventHandlerFactory$(PreprocessSuffix): EventHandlerFactory.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/EventHandlerFactory$(PreprocessSuffix) "/root/XGame/XNetFrame/EventHandlerFactory.cpp"

$(IntermediateDirectory)/StringTokenizer$(ObjectSuffix): StringTokenizer.cpp $(IntermediateDirectory)/StringTokenizer$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/StringTokenizer.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/StringTokenizer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StringTokenizer$(DependSuffix): StringTokenizer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/StringTokenizer$(ObjectSuffix) -MF$(IntermediateDirectory)/StringTokenizer$(DependSuffix) -MM "/root/XGame/XNetFrame/StringTokenizer.cpp"

$(IntermediateDirectory)/StringTokenizer$(PreprocessSuffix): StringTokenizer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StringTokenizer$(PreprocessSuffix) "/root/XGame/XNetFrame/StringTokenizer.cpp"

$(IntermediateDirectory)/Stream$(ObjectSuffix): Stream.cpp $(IntermediateDirectory)/Stream$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Stream.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Stream$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Stream$(DependSuffix): Stream.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Stream$(ObjectSuffix) -MF$(IntermediateDirectory)/Stream$(DependSuffix) -MM "/root/XGame/XNetFrame/Stream.cpp"

$(IntermediateDirectory)/Stream$(PreprocessSuffix): Stream.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Stream$(PreprocessSuffix) "/root/XGame/XNetFrame/Stream.cpp"

$(IntermediateDirectory)/System$(ObjectSuffix): System.cpp $(IntermediateDirectory)/System$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/System.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/System$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/System$(DependSuffix): System.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/System$(ObjectSuffix) -MF$(IntermediateDirectory)/System$(DependSuffix) -MM "/root/XGame/XNetFrame/System.cpp"

$(IntermediateDirectory)/System$(PreprocessSuffix): System.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/System$(PreprocessSuffix) "/root/XGame/XNetFrame/System.cpp"

$(IntermediateDirectory)/Socket$(ObjectSuffix): Socket.cpp $(IntermediateDirectory)/Socket$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Socket.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Socket$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Socket$(DependSuffix): Socket.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Socket$(ObjectSuffix) -MF$(IntermediateDirectory)/Socket$(DependSuffix) -MM "/root/XGame/XNetFrame/Socket.cpp"

$(IntermediateDirectory)/Socket$(PreprocessSuffix): Socket.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Socket$(PreprocessSuffix) "/root/XGame/XNetFrame/Socket.cpp"

$(IntermediateDirectory)/RotativeBuffer$(ObjectSuffix): RotativeBuffer.cpp $(IntermediateDirectory)/RotativeBuffer$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/RotativeBuffer.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/RotativeBuffer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RotativeBuffer$(DependSuffix): RotativeBuffer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/RotativeBuffer$(ObjectSuffix) -MF$(IntermediateDirectory)/RotativeBuffer$(DependSuffix) -MM "/root/XGame/XNetFrame/RotativeBuffer.cpp"

$(IntermediateDirectory)/RotativeBuffer$(PreprocessSuffix): RotativeBuffer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RotativeBuffer$(PreprocessSuffix) "/root/XGame/XNetFrame/RotativeBuffer.cpp"

$(IntermediateDirectory)/SocketImpl$(ObjectSuffix): SocketImpl.cpp $(IntermediateDirectory)/SocketImpl$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/SocketImpl.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/SocketImpl$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SocketImpl$(DependSuffix): SocketImpl.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/SocketImpl$(ObjectSuffix) -MF$(IntermediateDirectory)/SocketImpl$(DependSuffix) -MM "/root/XGame/XNetFrame/SocketImpl.cpp"

$(IntermediateDirectory)/SocketImpl$(PreprocessSuffix): SocketImpl.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SocketImpl$(PreprocessSuffix) "/root/XGame/XNetFrame/SocketImpl.cpp"

$(IntermediateDirectory)/UdpThread$(ObjectSuffix): UdpThread.cpp $(IntermediateDirectory)/UdpThread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/UdpThread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/UdpThread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UdpThread$(DependSuffix): UdpThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/UdpThread$(ObjectSuffix) -MF$(IntermediateDirectory)/UdpThread$(DependSuffix) -MM "/root/XGame/XNetFrame/UdpThread.cpp"

$(IntermediateDirectory)/UdpThread$(PreprocessSuffix): UdpThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UdpThread$(PreprocessSuffix) "/root/XGame/XNetFrame/UdpThread.cpp"

$(IntermediateDirectory)/SessionFactory$(ObjectSuffix): SessionFactory.cpp $(IntermediateDirectory)/SessionFactory$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/SessionFactory.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/SessionFactory$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SessionFactory$(DependSuffix): SessionFactory.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/SessionFactory$(ObjectSuffix) -MF$(IntermediateDirectory)/SessionFactory$(DependSuffix) -MM "/root/XGame/XNetFrame/SessionFactory.cpp"

$(IntermediateDirectory)/SessionFactory$(PreprocessSuffix): SessionFactory.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SessionFactory$(PreprocessSuffix) "/root/XGame/XNetFrame/SessionFactory.cpp"

$(IntermediateDirectory)/MessageDigest$(ObjectSuffix): MessageDigest.cpp $(IntermediateDirectory)/MessageDigest$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/MessageDigest.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/MessageDigest$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MessageDigest$(DependSuffix): MessageDigest.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/MessageDigest$(ObjectSuffix) -MF$(IntermediateDirectory)/MessageDigest$(DependSuffix) -MM "/root/XGame/XNetFrame/MessageDigest.cpp"

$(IntermediateDirectory)/MessageDigest$(PreprocessSuffix): MessageDigest.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MessageDigest$(PreprocessSuffix) "/root/XGame/XNetFrame/MessageDigest.cpp"

$(IntermediateDirectory)/MessageHandlerFactory$(ObjectSuffix): MessageHandlerFactory.cpp $(IntermediateDirectory)/MessageHandlerFactory$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/MessageHandlerFactory.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/MessageHandlerFactory$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MessageHandlerFactory$(DependSuffix): MessageHandlerFactory.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/MessageHandlerFactory$(ObjectSuffix) -MF$(IntermediateDirectory)/MessageHandlerFactory$(DependSuffix) -MM "/root/XGame/XNetFrame/MessageHandlerFactory.cpp"

$(IntermediateDirectory)/MessageHandlerFactory$(PreprocessSuffix): MessageHandlerFactory.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MessageHandlerFactory$(PreprocessSuffix) "/root/XGame/XNetFrame/MessageHandlerFactory.cpp"

$(IntermediateDirectory)/InetAddress$(ObjectSuffix): InetAddress.cpp $(IntermediateDirectory)/InetAddress$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/InetAddress.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/InetAddress$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InetAddress$(DependSuffix): InetAddress.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/InetAddress$(ObjectSuffix) -MF$(IntermediateDirectory)/InetAddress$(DependSuffix) -MM "/root/XGame/XNetFrame/InetAddress.cpp"

$(IntermediateDirectory)/InetAddress$(PreprocessSuffix): InetAddress.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InetAddress$(PreprocessSuffix) "/root/XGame/XNetFrame/InetAddress.cpp"

$(IntermediateDirectory)/ScheduleService$(ObjectSuffix): ScheduleService.cpp $(IntermediateDirectory)/ScheduleService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/ScheduleService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/ScheduleService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScheduleService$(DependSuffix): ScheduleService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/ScheduleService$(ObjectSuffix) -MF$(IntermediateDirectory)/ScheduleService$(DependSuffix) -MM "/root/XGame/XNetFrame/ScheduleService.cpp"

$(IntermediateDirectory)/ScheduleService$(PreprocessSuffix): ScheduleService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScheduleService$(PreprocessSuffix) "/root/XGame/XNetFrame/ScheduleService.cpp"

$(IntermediateDirectory)/Profiler$(ObjectSuffix): Profiler.cpp $(IntermediateDirectory)/Profiler$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Profiler.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Profiler$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Profiler$(DependSuffix): Profiler.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Profiler$(ObjectSuffix) -MF$(IntermediateDirectory)/Profiler$(DependSuffix) -MM "/root/XGame/XNetFrame/Profiler.cpp"

$(IntermediateDirectory)/Profiler$(PreprocessSuffix): Profiler.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Profiler$(PreprocessSuffix) "/root/XGame/XNetFrame/Profiler.cpp"

$(IntermediateDirectory)/FileLoader$(ObjectSuffix): FileLoader.cpp $(IntermediateDirectory)/FileLoader$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/FileLoader.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/FileLoader$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileLoader$(DependSuffix): FileLoader.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/FileLoader$(ObjectSuffix) -MF$(IntermediateDirectory)/FileLoader$(DependSuffix) -MM "/root/XGame/XNetFrame/FileLoader.cpp"

$(IntermediateDirectory)/FileLoader$(PreprocessSuffix): FileLoader.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileLoader$(PreprocessSuffix) "/root/XGame/XNetFrame/FileLoader.cpp"

$(IntermediateDirectory)/ServerSocket$(ObjectSuffix): ServerSocket.cpp $(IntermediateDirectory)/ServerSocket$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/ServerSocket.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/ServerSocket$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ServerSocket$(DependSuffix): ServerSocket.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/ServerSocket$(ObjectSuffix) -MF$(IntermediateDirectory)/ServerSocket$(DependSuffix) -MM "/root/XGame/XNetFrame/ServerSocket.cpp"

$(IntermediateDirectory)/ServerSocket$(PreprocessSuffix): ServerSocket.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ServerSocket$(PreprocessSuffix) "/root/XGame/XNetFrame/ServerSocket.cpp"

$(IntermediateDirectory)/UdpSocket$(ObjectSuffix): UdpSocket.cpp $(IntermediateDirectory)/UdpSocket$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/UdpSocket.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/UdpSocket$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UdpSocket$(DependSuffix): UdpSocket.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/UdpSocket$(ObjectSuffix) -MF$(IntermediateDirectory)/UdpSocket$(DependSuffix) -MM "/root/XGame/XNetFrame/UdpSocket.cpp"

$(IntermediateDirectory)/UdpSocket$(PreprocessSuffix): UdpSocket.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UdpSocket$(PreprocessSuffix) "/root/XGame/XNetFrame/UdpSocket.cpp"

$(IntermediateDirectory)/FileViewer$(ObjectSuffix): FileViewer.cpp $(IntermediateDirectory)/FileViewer$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/FileViewer.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/FileViewer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileViewer$(DependSuffix): FileViewer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/FileViewer$(ObjectSuffix) -MF$(IntermediateDirectory)/FileViewer$(DependSuffix) -MM "/root/XGame/XNetFrame/FileViewer.cpp"

$(IntermediateDirectory)/FileViewer$(PreprocessSuffix): FileViewer.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileViewer$(PreprocessSuffix) "/root/XGame/XNetFrame/FileViewer.cpp"

$(IntermediateDirectory)/ScheduleThread$(ObjectSuffix): ScheduleThread.cpp $(IntermediateDirectory)/ScheduleThread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/ScheduleThread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/ScheduleThread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ScheduleThread$(DependSuffix): ScheduleThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/ScheduleThread$(ObjectSuffix) -MF$(IntermediateDirectory)/ScheduleThread$(DependSuffix) -MM "/root/XGame/XNetFrame/ScheduleThread.cpp"

$(IntermediateDirectory)/ScheduleThread$(PreprocessSuffix): ScheduleThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ScheduleThread$(PreprocessSuffix) "/root/XGame/XNetFrame/ScheduleThread.cpp"

$(IntermediateDirectory)/LogThread$(ObjectSuffix): LogThread.cpp $(IntermediateDirectory)/LogThread$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/LogThread.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/LogThread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LogThread$(DependSuffix): LogThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/LogThread$(ObjectSuffix) -MF$(IntermediateDirectory)/LogThread$(DependSuffix) -MM "/root/XGame/XNetFrame/LogThread.cpp"

$(IntermediateDirectory)/LogThread$(PreprocessSuffix): LogThread.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LogThread$(PreprocessSuffix) "/root/XGame/XNetFrame/LogThread.cpp"

$(IntermediateDirectory)/Session$(ObjectSuffix): Session.cpp $(IntermediateDirectory)/Session$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Session.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Session$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Session$(DependSuffix): Session.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Session$(ObjectSuffix) -MF$(IntermediateDirectory)/Session$(DependSuffix) -MM "/root/XGame/XNetFrame/Session.cpp"

$(IntermediateDirectory)/Session$(PreprocessSuffix): Session.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Session$(PreprocessSuffix) "/root/XGame/XNetFrame/Session.cpp"

$(IntermediateDirectory)/UdpService$(ObjectSuffix): UdpService.cpp $(IntermediateDirectory)/UdpService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/UdpService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/UdpService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UdpService$(DependSuffix): UdpService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/UdpService$(ObjectSuffix) -MF$(IntermediateDirectory)/UdpService$(DependSuffix) -MM "/root/XGame/XNetFrame/UdpService.cpp"

$(IntermediateDirectory)/UdpService$(PreprocessSuffix): UdpService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UdpService$(PreprocessSuffix) "/root/XGame/XNetFrame/UdpService.cpp"

$(IntermediateDirectory)/Random$(ObjectSuffix): Random.cpp $(IntermediateDirectory)/Random$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/Random.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Random$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Random$(DependSuffix): Random.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/Random$(ObjectSuffix) -MF$(IntermediateDirectory)/Random$(DependSuffix) -MM "/root/XGame/XNetFrame/Random.cpp"

$(IntermediateDirectory)/Random$(PreprocessSuffix): Random.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Random$(PreprocessSuffix) "/root/XGame/XNetFrame/Random.cpp"

$(IntermediateDirectory)/XNetConnector$(ObjectSuffix): XNetConnector.cpp $(IntermediateDirectory)/XNetConnector$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/XNetConnector.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/XNetConnector$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/XNetConnector$(DependSuffix): XNetConnector.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/XNetConnector$(ObjectSuffix) -MF$(IntermediateDirectory)/XNetConnector$(DependSuffix) -MM "/root/XGame/XNetFrame/XNetConnector.cpp"

$(IntermediateDirectory)/XNetConnector$(PreprocessSuffix): XNetConnector.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/XNetConnector$(PreprocessSuffix) "/root/XGame/XNetFrame/XNetConnector.cpp"

$(IntermediateDirectory)/XNetChannel$(ObjectSuffix): XNetChannel.cpp $(IntermediateDirectory)/XNetChannel$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/XNetChannel.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/XNetChannel$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/XNetChannel$(DependSuffix): XNetChannel.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/XNetChannel$(ObjectSuffix) -MF$(IntermediateDirectory)/XNetChannel$(DependSuffix) -MM "/root/XGame/XNetFrame/XNetChannel.cpp"

$(IntermediateDirectory)/XNetChannel$(PreprocessSuffix): XNetChannel.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/XNetChannel$(PreprocessSuffix) "/root/XGame/XNetFrame/XNetChannel.cpp"

$(IntermediateDirectory)/SessionDispatchService$(ObjectSuffix): SessionDispatchService.cpp $(IntermediateDirectory)/SessionDispatchService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/SessionDispatchService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/SessionDispatchService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SessionDispatchService$(DependSuffix): SessionDispatchService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/SessionDispatchService$(ObjectSuffix) -MF$(IntermediateDirectory)/SessionDispatchService$(DependSuffix) -MM "/root/XGame/XNetFrame/SessionDispatchService.cpp"

$(IntermediateDirectory)/SessionDispatchService$(PreprocessSuffix): SessionDispatchService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SessionDispatchService$(PreprocessSuffix) "/root/XGame/XNetFrame/SessionDispatchService.cpp"

$(IntermediateDirectory)/ChannelDispatchService$(ObjectSuffix): ChannelDispatchService.cpp $(IntermediateDirectory)/ChannelDispatchService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/ChannelDispatchService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/ChannelDispatchService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ChannelDispatchService$(DependSuffix): ChannelDispatchService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/ChannelDispatchService$(ObjectSuffix) -MF$(IntermediateDirectory)/ChannelDispatchService$(DependSuffix) -MM "/root/XGame/XNetFrame/ChannelDispatchService.cpp"

$(IntermediateDirectory)/ChannelDispatchService$(PreprocessSuffix): ChannelDispatchService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ChannelDispatchService$(PreprocessSuffix) "/root/XGame/XNetFrame/ChannelDispatchService.cpp"

$(IntermediateDirectory)/ContextDispatchService$(ObjectSuffix): ContextDispatchService.cpp $(IntermediateDirectory)/ContextDispatchService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/ContextDispatchService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/ContextDispatchService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ContextDispatchService$(DependSuffix): ContextDispatchService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/ContextDispatchService$(ObjectSuffix) -MF$(IntermediateDirectory)/ContextDispatchService$(DependSuffix) -MM "/root/XGame/XNetFrame/ContextDispatchService.cpp"

$(IntermediateDirectory)/ContextDispatchService$(PreprocessSuffix): ContextDispatchService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ContextDispatchService$(PreprocessSuffix) "/root/XGame/XNetFrame/ContextDispatchService.cpp"

$(IntermediateDirectory)/XNetService$(ObjectSuffix): XNetService.cpp $(IntermediateDirectory)/XNetService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/XNetService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/XNetService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/XNetService$(DependSuffix): XNetService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/XNetService$(ObjectSuffix) -MF$(IntermediateDirectory)/XNetService$(DependSuffix) -MM "/root/XGame/XNetFrame/XNetService.cpp"

$(IntermediateDirectory)/XNetService$(PreprocessSuffix): XNetService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/XNetService$(PreprocessSuffix) "/root/XGame/XNetFrame/XNetService.cpp"

$(IntermediateDirectory)/XScheduleService$(ObjectSuffix): XScheduleService.cpp $(IntermediateDirectory)/XScheduleService$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/XScheduleService.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/XScheduleService$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/XScheduleService$(DependSuffix): XScheduleService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/XScheduleService$(ObjectSuffix) -MF$(IntermediateDirectory)/XScheduleService$(DependSuffix) -MM "/root/XGame/XNetFrame/XScheduleService.cpp"

$(IntermediateDirectory)/XScheduleService$(PreprocessSuffix): XScheduleService.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/XScheduleService$(PreprocessSuffix) "/root/XGame/XNetFrame/XScheduleService.cpp"

$(IntermediateDirectory)/TimerEventHandler$(ObjectSuffix): TimerEventHandler.cpp $(IntermediateDirectory)/TimerEventHandler$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/root/XGame/XNetFrame/TimerEventHandler.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/TimerEventHandler$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TimerEventHandler$(DependSuffix): TimerEventHandler.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/TimerEventHandler$(ObjectSuffix) -MF$(IntermediateDirectory)/TimerEventHandler$(DependSuffix) -MM "/root/XGame/XNetFrame/TimerEventHandler.cpp"

$(IntermediateDirectory)/TimerEventHandler$(PreprocessSuffix): TimerEventHandler.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TimerEventHandler$(PreprocessSuffix) "/root/XGame/XNetFrame/TimerEventHandler.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/AdjustableBuffer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/AdjustableBuffer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/AdjustableBuffer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/AuthCrypt$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/AuthCrypt$(DependSuffix)
	$(RM) $(IntermediateDirectory)/AuthCrypt$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Channel$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Channel$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Channel$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/BitStream$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/BitStream$(DependSuffix)
	$(RM) $(IntermediateDirectory)/BitStream$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ConsoleViewer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ConsoleViewer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ConsoleViewer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Context$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Context$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Context$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DispatchService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DispatchService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DispatchService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DispatchThread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DispatchThread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DispatchThread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/EpollAcceptThread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/EpollAcceptThread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/EpollAcceptThread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/EpollAsyncIoThread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/EpollAsyncIoThread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/EpollAsyncIoThread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/EpollService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/EpollService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/EpollService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/EpollImpl$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/EpollImpl$(DependSuffix)
	$(RM) $(IntermediateDirectory)/EpollImpl$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Event$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Event$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Event$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/XNetSession$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/XNetSession$(DependSuffix)
	$(RM) $(IntermediateDirectory)/XNetSession$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TcpService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TcpService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TcpService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Loader$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Loader$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Loader$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Log$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Log$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Log$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Exception$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Exception$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Exception$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Thread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Thread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Thread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TcpSocket$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TcpSocket$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TcpSocket$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Monitor$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Monitor$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Monitor$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/MessageHandler$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/MessageHandler$(DependSuffix)
	$(RM) $(IntermediateDirectory)/MessageHandler$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Viewer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Viewer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Viewer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Properties$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Properties$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Properties$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TcpThread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TcpThread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TcpThread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ResourceInterface$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ResourceInterface$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ResourceInterface$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/EventHandlerFactory$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/EventHandlerFactory$(DependSuffix)
	$(RM) $(IntermediateDirectory)/EventHandlerFactory$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/StringTokenizer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/StringTokenizer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/StringTokenizer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Stream$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Stream$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Stream$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/System$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/System$(DependSuffix)
	$(RM) $(IntermediateDirectory)/System$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Socket$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Socket$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Socket$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/RotativeBuffer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/RotativeBuffer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/RotativeBuffer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/SocketImpl$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/SocketImpl$(DependSuffix)
	$(RM) $(IntermediateDirectory)/SocketImpl$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/UdpThread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/UdpThread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/UdpThread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/SessionFactory$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/SessionFactory$(DependSuffix)
	$(RM) $(IntermediateDirectory)/SessionFactory$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/MessageDigest$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/MessageDigest$(DependSuffix)
	$(RM) $(IntermediateDirectory)/MessageDigest$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/MessageHandlerFactory$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/MessageHandlerFactory$(DependSuffix)
	$(RM) $(IntermediateDirectory)/MessageHandlerFactory$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/InetAddress$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/InetAddress$(DependSuffix)
	$(RM) $(IntermediateDirectory)/InetAddress$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ScheduleService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ScheduleService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ScheduleService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Profiler$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Profiler$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Profiler$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/FileLoader$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/FileLoader$(DependSuffix)
	$(RM) $(IntermediateDirectory)/FileLoader$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ServerSocket$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ServerSocket$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ServerSocket$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/UdpSocket$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/UdpSocket$(DependSuffix)
	$(RM) $(IntermediateDirectory)/UdpSocket$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/FileViewer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/FileViewer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/FileViewer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ScheduleThread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ScheduleThread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ScheduleThread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/LogThread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/LogThread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/LogThread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Session$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Session$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Session$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/UdpService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/UdpService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/UdpService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Random$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Random$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Random$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/XNetConnector$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/XNetConnector$(DependSuffix)
	$(RM) $(IntermediateDirectory)/XNetConnector$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/XNetChannel$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/XNetChannel$(DependSuffix)
	$(RM) $(IntermediateDirectory)/XNetChannel$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/SessionDispatchService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/SessionDispatchService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/SessionDispatchService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ChannelDispatchService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ChannelDispatchService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ChannelDispatchService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ContextDispatchService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ContextDispatchService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ContextDispatchService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/XNetService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/XNetService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/XNetService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/XScheduleService$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/XScheduleService$(DependSuffix)
	$(RM) $(IntermediateDirectory)/XScheduleService$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TimerEventHandler$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TimerEventHandler$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TimerEventHandler$(PreprocessSuffix)
	$(RM) $(OutputFile)


