#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
#include <nomadmemory.au3>

;GLOBAL VARIABLES SECTION ==========================================================================
Global $botting = 0 ; Ботим , или нет.
Global $attack=0,$meditation=0,$target,$Paused
Global $ProcessName = "Martial Empires" ;Название окна
Global $ProcessID = WinGetProcess($ProcessName,"") ;Ну или $Pid кому как нравится
Global $HP = 0x00CBDFB8 ;
Global $x_coord_adr,$y_coord_adr ;
Global $DllInformation = _MemoryOpen($ProcessID)
Global $HP_percent,$MP_percent,$Meditation_time
Global $HPdec,$HP_before_meditation ; Хп персонажа
Global $med_time ; Время Медитации
Global $i=0 ; Проверка на агр при медитации
Global $potion_key,$potion_key_read,$use_potions,$use_potions_read,$hp_when_use_potions,$hp_when_use_potions_read
$meditation_key="{F11}"
;GLOBAL VARIABLES  SECTION ==========================================================================

;GUI _ INTERFACE_______________________________________________________________________________________
Opt("GUIOnEventMode", 1)  ; Change to OnEvent mode
Global $botting = 0
$main_window = GUICreate("Martial Empires Bot v 0.2.5",500,500)                 ; start the definition
GUISetBkColor(0x74B5D3)
$bot_start = GUICtrlCreateButton("Start Botting", 175,2 ,150,30) ; Запуск бота
$bot_stop = GUICtrlCreateButton("Stop  Botting", 175,35 ,150,30) ; Остановка бота

$use_potions = GUICtrlCreateCheckbox("Use Potions", 4,10)  ; Кнопка использования потов
GUICtrlCreateLabel("KEY= (0-9) ", 4,35)
$potion_key = GUICtrlCreateInput("",4,50,50,20) ; Кнопка от 0 до 9 на которой у вас висит банка хп
GUICtrlCreateLabel("HP < ", 90,35)
$hp_when_use_potions = GUICtrlCreateInput("",80,50,50,20)


$use_skills = GUICtrlCreateCheckbox("Use Skills", 400,10)  ; Кнопка использования скиллов
GUICtrlCreateLabel("Using Meditation when HP < ", 4,80) ; Настраиваем хп, при котором используется медитация
$hp_when_meditation_gui = GUICtrlCreateInput("",4,100,50,20)
GUICtrlCreateLabel("Time for meditation (in Sec) ", 4,125) ; Указываем время медитации в секундах
$Meditation_time = GUICtrlCreateInput("",4,145,50,20)
GUISetOnEvent($GUI_EVENT_CLOSE, "CLOSE_PROGRAM")
GUICtrlSetOnEvent($bot_start, "BOT_START")
GUICtrlSetOnEvent($bot_stop, "BOT_STOP")
HotKeySet("!s","BOT_STOP")
GUISetState(@SW_SHOW)
;GUI _ INTERFACE_______________________________________________________________________________________




Func CLOSE_PROGRAM()
      Exit
EndFunc

Func BOT_START()
	WinActivate("Martial Empires")
    ToolTip("Bot active", 350, 0) ;Включить надпись
	sleep(5000)
    $botting = 1 ;Включить бота
EndFunc

Func BOT_STOP()
    ToolTip("") ;Выключить надпись
    $botting = 0 ;Выключить бота
	$meditation = 1 ;Выключить бота
EndFunc


; Проверяем на аггр
Func aggro()
	ToolTip('Проверяем на аггр',350,0)
	$check_aggro=PixelGetColor(841,55)
	$check_hex_aggro=Hex($check_aggro, 6)
	$check_aggro2=PixelGetColor(841,55)
	$check_hex_aggro2=Hex($check_aggro2, 6)
	$check_aggro3=PixelGetColor(841,55)
	$check_hex_aggro3=Hex($check_aggro3, 6)
			if  $check_hex_aggro = "ABA79A" or $check_hex_aggro2 = "050406" or $check_hex_aggro3 = "A09270" then
				Return True
			Else
				Return False
			EndIf
EndFunc

Func new_aggro()
	ToolTip('Проверяем на аггр',350,0)
	$new_aggro=PixelGetColor(808,42)
	$new_hex_aggro=Hex($new_aggro, 6)
			if  $new_hex_aggro = "970308"   then
					Return True
			Else
					Return False
			EndIf
EndFunc

;Количество ХП
Func hp()
	$CurrentHP = (_MemoryRead(0x00CBDFB8, $DllInformation, 'ptr'))
    $HPdec = dec(StringTrimLeft($CurrentHP, 2))
	Return $HPdec
EndFunc

; Проверка ХП
Func check_hp()

	$CurrentHP = (_MemoryRead(0x00CBDFB8, $DllInformation, 'ptr'))
    If @Error Then
        MsgBox(4096, "ERROR", "Failed to read memory.")
        Exit
    EndIf
    $HPdec = dec(StringTrimLeft($CurrentHP, 2))
	$hp_when_meditation=GUICtrlRead($hp_when_meditation_gui)
	SLEEP(Random(100,300))
	if $HPdec < $hp_when_meditation Then
		return True
    Else
		return False
	EndIf



EndFunc


Func check_mp()
	$CurrentMP = (_MemoryRead($HP, $DllInformation, 'ptr'))
    If @Error Then
        MsgBox(4096, "ERROR", "Failed to read memory.")
        Exit
    EndIf
    Global $MPdec = dec(StringTrimLeft($CurrentMP, 2))
	ToolTip($MPdec,350,0)
	SLEEP(5000)

EndFunc



; HP < 50% ?!
Func low_hp()
	ToolTip('Проверяем на хп',350,0)
	$check=PixelGetColor(457,989)
	$check_hex_hp=Hex($check, 6)
			if  $check_hex_hp = "F94546" then
				Return False
			Else
				Return True
			EndIf

EndFunc



; НЕ ГОТОВО !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Func aggro_mob()
	ToolTip('Проверяем на аггр',350,0)
	$aggro_mob=PixelGetColor(784,17)
	$aggro_mob_hex=Hex($aggro_mob, 6)
			if  $aggro_mob_hex = "951716" then
					Return False
			Else
					Return True
			EndIf

EndFunc



; Медитация
Func meditation()


$hp_before=hp()
ToolTip('Медитация',350,0)
Sleep(Random(50,1000))
Send("{F10}")
sleep(500)
		while  $i<$med_time/1000



			if aggro()=0 Then
				sleep(1000)
				$i=$i+1
				;Elseif hp() < $hp_before_meditation+100	Then
				;$i=0;
				;meditation()
			Else
				$meditation=0
				ExitLoop
			EndIf
		WEnd
$meditation=0
EndFunc

Func MyExit()
    Exit
EndFunc



;Бьем ))))
Func use_skills()
	ToolTip('Юзаем скиллы',350,0)
	Sleep(200)
	Send("{F6}")  ; >>> TO FAST
	Sleep(500)
	Send("{F1}")
	Sleep(500)
	Send("{F2}")
	Sleep(500)
	Send("`")
	Sleep(500)
	Send("{F3}")
	Sleep(500)
	Send("{F4}")

EndFunc



;hp<50 and not_aggro
Func heal()
	if low_hp()=1 and aggro()=0 Then
		return 1
	Else
		Return 0
	EndIf
EndFunc


Func bot_working()
	While $botting = 1
			;Sleep(500)
			if check_hp() = 1  Then ; Хп < 3500, если да то $meditation=1 and aggro()=0
					$meditation=1
			Else
					$meditation=0
			EndIf


					if $meditation=1 and aggro()=0 Then ; Если $meditation=1 юзаем скилл , после использования $meditation=0
						$i=0
						meditation()
						;sleep(500)
					else
						$meditation=0
					EndIf


							while $meditation=0 ;Если $meditation=0, атакуем мобов
									$time=Random(400,800)
									ToolTip('Следующая цель',350,0)
									;Sleep(300)
									sleep($time)
									send("f")
									sleep(500)
									$mob_hp_before_attack=hp()
									use_skills()
									if check_hp()=1 Then  ;Бьем мобов, если вало хп запускаем цикл проверки , мертв ли моб или нет.
														while aggro()=1 and $botting=1
															  ToolTip('Добиваем моба',350,0)
															  if $HPdec < $hp_when_use_potions_read and $use_potions_read=1 Then
																sleep(Random(100,500))
																send($potion_key_read)
															  EndIf

															  if $HPdec < 500 Then
																sleep(Random(100,500))
																Send("!1")
															  EndIf
														use_skills()
														sleep(2000)
														;$hp_before_meditation=hp()
														WEnd
									;Sleep(250)
									$meditation=1
									;Sleep(250)
									$attack=0
									EndIf
							WEnd

WEnd
EndFunc






;; MAIN _ PROGRAM
While 1
	$med_time=GUICtrlRead($Meditation_time)*1000  ; Считываем // продолжительность медитации
	$use_potions_read=GUICtrlRead($use_potions)	  ; Считываем // использовать ли банки ХП
	$potion_key_read=GUICtrlRead($potion_key)     ; Считываем // кнопку, на которой висит банка ХП
	$hp_when_use_potions_read=GUICtrlRead($hp_when_use_potions) ; Считываем // Кол-во хп при котором использовать банку
    If $botting = 1 Then ;Если бот включен
        bot_working() ;Работа бота
	Else
	ToolTip("Bot disabled", 350, 0) ;Включить надпись
    sleep(1000)
	ToolTip("Activate me sir ^_^:)", 350, 0) ;Включить надпись
	sleep(1000)
    EndIf
Wend