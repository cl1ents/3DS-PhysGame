@echo off
cd PhysGame
make cia
cd ..

echo Attempting to serve CIA over local wifi...
for /f "delims=[] tokens=2" %%a in ('ping -4 -n 1 %ComputerName% ^| findstr [') do set NetworkIP=%%a
curl qrenco.de/%NetworkIP%:8000/PhysGame.cia
echo Scan this QR code using your 3DS :3
echo Then feel free to CTRL+C
python -m http.server --directory .\PhysGame\output\