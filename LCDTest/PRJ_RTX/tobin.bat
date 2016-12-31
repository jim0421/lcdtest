"C:\Keil\ARM\ARMCC\bin\fromelf" --bin --output=..\ZBinFile\LiftLcd_Temp.t ..\ZObjLst\LiftLcd_S4.axf
copy /b  ..\ZBinFile\LiftLcd_Temp.t+..\ZBinFile\padding.raw ..\ZBinFile\LiftLcd_Test.bin
pause;
