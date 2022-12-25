:: SCRIPT A32 - CST8152 - Fall 2021
::
:: COMPILERS COURSE - SCRIPT ---------------------------------------------

CLS

SET COMPILER=Compilers.exe

SET FILE1=A22A_Empty
SET FILE2=A22B_Hello
SET FILE3=A22C_Expression

REM ---------------------------------------------------------------------
REM - Begin of Tests (A32 - F21) ----------------------------------------
REM ---------------------------------------------------------------------

ren *.exe %COMPILER%

::
:: BASIC TESTS  ----------------------------------------------------------
::
REM - Basic Tests (A32 - F21) - - - - - - - - - - - - - - - - - - - - - -

%COMPILER% 3 %FILE1%.gph	> %FILE1%.out	2> %FILE1%.err
%COMPILER% 3 %FILE2%.gph	> %FILE2%.out	2> %FILE2%.err
%COMPILER% 3 %FILE3%.gph	> %FILE3%.out	2> %FILE3%.err

ECHO "Showing A32 Results..."

ECHO =========================
TYPE %FILE1%.out
ECHO .........................
TYPE %FILE2%.out
ECHO .........................
TYPE %FILE3%.out
ECHO =========================
DIR *.out
DIR *.err

REM ---------------------------------------------------------------------
REM - End of Tests (A32 - F21) ------------------------------------------
REM ---------------------------------------------------------------------
