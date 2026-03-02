% Zadani c. 9:
% Napiste program resici ukol dany predikatem u9(LIN, VOUT), kde LIN je vstupni 
% ciselny seznam s nejmene dvema prvky a VOUT je vystupni promenna, ve ktere
% se vraci absolutni hodnota maximalniho rozdilu mezi dvema sousednimi prvky 
% v seznamu LIN. 

% Testovaci predikaty:                                          % VOUT
u9_1:- u9([5,27.5,-1,28.3,19,-4],VOUT),write(VOUT).           	% 29.3
u9_2:- u9([1.2,3,5,-7.4],VOUT),write(VOUT).            		% 12.4
u9_3:- u9([1,3,5,7],VOUT),write(VOUT).                		% 2
u9_r:- write('Zadej LIN: '),read(LIN),u9(LIN,VOUT),write(VOUT).

abs_val(X, Y) :- X < 0, Y is -X.
abs_val(X, Y) :- X >= 0, Y is X.

max_diff([], Max, Max).
max_diff([_], Max, Max).


max_diff([H1,H2|T], X, Y) :- Diff is H2 - H1, abs_val(Diff, Absdiff), Newmax is max(Absdiff, X), max_diff([H2|T], Newmax, Y).

% Reseni:
u9(LIN,VOUT):- max_diff(LIN, 0, MaxDiff),
VOUT = MaxDiff.