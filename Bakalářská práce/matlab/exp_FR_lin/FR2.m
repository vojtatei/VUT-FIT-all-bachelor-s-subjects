addpath('../methods/');
% Definice časového intervalu a parametrů
T = 2; % konečný čas
omega = 2*pi/T; % frekvence
h = 0.4; % krok pro Taylorovu metodu
eps = 1e-10; % tolerance
tspan = [0 T];

% Analytické hodnoty Fourierových koeficientů
A0 = 1;
A2 = -0.5;
analytical_solution = [A0; A2];

% Definice matice A pro lineární části systému
n = 7; % počet rovnic
A = zeros(n,n);
A(1,3) = 1;
A(3,4) = 2*omega;
A(4,5) = omega;
A(4,3) = -omega;
A(5,4) = -2*omega;
A(6,7) = -2*omega;
A(7,6) = 2*omega;

% Definice matice B1 a indexů pro kvadratické členy

B1 = zeros(n, 1);
B1(2,1) = 2/T;
% Indices pro y3 * y6
y_ij = [3 6];

% Počáteční podmínky
y0 = zeros(n,1);
y0(5) = 1; % y₅(0) = 1
y0(6) = 1; % y₆(0) = 1

% Definice pravé strany pro ODE solvery
f = @(t,y) [y(3);
           (2/T)*y(3)*y(6); % nelineární člen
           2*omega*y(4);
           omega*(y(5) - y(3));
           -2*omega*y(4);
           -2*omega*y(7);
           2*omega*y(6)];

% Definice konstantního vektoru b
b = zeros(n,1);

% Měření času a výpočet pomocí Taylorovy metody (nelineární)
tic;
[t_taylor, y_taylor, ord] = taylor_method_nonlinear(A, B1, b, y_ij, y0, h, T, eps, 60);
time_taylor = toc;

% Měření času a výpočet pomocí vestavěných solverů
options = odeset('RelTol', eps, 'AbsTol', eps*ones(1,n));

% ode23
tic;
[t_ode23, y_ode23] = ode23(f, tspan, y0, options);
time_ode23 = toc;

% ode45
tic;
[t_ode45, y_ode45] = ode45(f, tspan, y0, options);
time_ode45 = toc;

% ode78
tic;
[t_ode78, y_ode78] = ode78(f, tspan, y0, options);
time_ode78 = toc;

% ode89
tic;
[t_ode89, y_ode89] = ode89(f, tspan, y0, options);
time_ode89 = toc;

% ode113
tic;
[t_ode113, y_ode113] = ode113(f, tspan, y0, options);
time_ode113 = toc;

% Výpočet hodnot pro jednotlivé metody
numerical_solution_taylor = [y_taylor(5,end); y_taylor(2,end)];
numerical_solution_ode23 = [y_ode23(end,5); y_ode23(end,2)];
numerical_solution_ode45 = [y_ode45(end,5); y_ode45(end,2)];
numerical_solution_ode78 = [y_ode78(end,5); y_ode78(end,2)];
numerical_solution_ode89 = [y_ode89(end,5); y_ode89(end,2)];
numerical_solution_ode113 = [y_ode113(end,5); y_ode113(end,2)];

% Debug výpis pro kontrolu hodnot
fprintf('\nKontrolní hodnoty:\n');
fprintf('Analytické řešení: A0 = %.6f, A2 = %.6f\n', A0, A2);
fprintf('Taylor: a0 = %.6f, a2 = %.6f\n', numerical_solution_taylor(1), numerical_solution_taylor(2));
fprintf('ODE23: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode23(1), numerical_solution_ode23(2));
fprintf('ODE45: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode45(1), numerical_solution_ode45(2));
fprintf('ODE78: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode78(1), numerical_solution_ode78(2));
fprintf('ODE89: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode89(1), numerical_solution_ode89(2));
fprintf('ODE113: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode113(1), numerical_solution_ode113(2));

% Výpočet chyb
error_taylor = norm(numerical_solution_taylor - analytical_solution);
error_ode23 = norm(numerical_solution_ode23 - analytical_solution);
error_ode45 = norm(numerical_solution_ode45 - analytical_solution);
error_ode78 = norm(numerical_solution_ode78 - analytical_solution);
error_ode89 = norm(numerical_solution_ode89 - analytical_solution);
error_ode113 = norm(numerical_solution_ode113 - analytical_solution);

% Výpočet poměrů času
ratio_ode23 = time_ode23/time_taylor;
ratio_ode45 = time_ode45/time_taylor;
ratio_ode78 = time_ode78/time_taylor;
ratio_ode89 = time_ode89/time_taylor;
ratio_ode113 = time_ode113/time_taylor;

% Vytvoření tabulky výsledků
fprintf('\nResults of calculations for Fourier coefficients:\n');
fprintf('--------------------------------------------------------\n');
fprintf('Solver\t\tTime [s]\t\tRatio\t\t||error||\n');
fprintf('--------------------------------------------------------\n');
fprintf('MTSM\t\t%.5e\t%.1f\t\t%.2e\n', time_taylor, 1.0, error_taylor);
fprintf('ode23\t\t%.5e\t%.1f\t\t%.2e\n', time_ode23, ratio_ode23, error_ode23);
fprintf('ode45\t\t%.5e\t%.1f\t\t%.2e\n', time_ode45, ratio_ode45, error_ode45);
fprintf('ode78\t\t%.5e\t%.1f\t\t%.2e\n', time_ode78, ratio_ode78, error_ode78);
fprintf('ode89\t\t%.5e\t%.1f\t\t%.2e\n', time_ode89, ratio_ode89, error_ode89);
fprintf('ode113\t\t%.5e\t%.1f\t\t%.2e\n', time_ode113, ratio_ode113, error_ode113);