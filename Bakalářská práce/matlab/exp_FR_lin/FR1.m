addpath('../methods/');
% Definice časového intervalu a parametrů
T = 2; % konečný čas
omega = 2*pi/T; % frekvence
h = 0.4; % krok pro Taylorovu metodu
h2 = 0.01;
eps = 1e-10; % tolerance
tspan = [0 T];

% Analytické hodnoty Fourierových koeficientů
A0 = 1;
A2 = -0.5;
analytical_solution = [A0; A2];

% Definice matice A pro lineární systém
n = 11; % počet rovnic
A = zeros(n,n);
% y'₁ = y₃
A(1,3) = 1;
% y'₂ = (2/T)y₆
A(2,6) = 2/T;
% y'₃ = 2ωy₄
A(3,4) = 2*omega;
% y'₄ = ω(y₅ - y₃)
A(4,5) = omega;
A(4,3) = -omega;
% y'₅ = -2ωy₄
A(5,4) = -2*omega;
% y'₆ = 2ω(y₈ - y₇)
A(6,8) = 2*omega;
A(6,7) = -2*omega;
% y'₇ = 2ω(y₉ + y₆)
A(7,9) = 2*omega;
A(7,6) = 2*omega;
% y'₈ = ω(y₁₀ - y₆ - 2y₉)
A(8,10) = omega;
A(8,6) = -omega;
A(8,9) = -2*omega;
% y'₉ = ω(y₁₁ - y₇ + 2y₈)
A(9,11) = omega;
A(9,7) = -omega;
A(9,8) = 2*omega;
% y'₁₀ = -2ω(y₈ + y₁₁)
A(10,8) = -2*omega;
A(10,11) = -2*omega;
% y'₁₁ = -2ω(y₉ - y₁₀)
A(11,9) = -2*omega;
A(11,10) = 2*omega;

% Počáteční podmínky
y0 = zeros(n,1);
y0(5) = 1; % y₅(0) = 1
y0(10) = 1; % y₁₀(0) = 1

% Definice pravé strany
b = zeros(n,1);

% Definice funkce pro ODE solvery
f = @(t,y) A*y + b;
options = odeset('RelTol', eps, 'AbsTol', eps*ones(1,n));

% Měření času a výpočet pomocí Taylorovy metody
tic;
[t_taylor, y_taylor] = taylor_method(A, b, y0, h, T, eps);
time_taylor = toc;

% Měření času a výpočet pomocí Eulerovy metody
tic;
[t_euler, y_euler] = euler_method(A, b, y0, h2, T);
time_euler = toc;

% Měření času a výpočet pomocí RK4 metody
tic;
[t_rk4, y_rk4] = rk4_method(A, b, y0, h2, T);
time_rk4 = toc;

% Měření času a výpočet pomocí vestavěných solverů
tic;
[t_ode23, y_ode23] = ode23(f, tspan, y0, options);
time_ode23 = toc;

tic;
[t_ode45, y_ode45] = ode45(f, tspan, y0, options);
time_ode45 = toc;

tic;
[t_ode78, y_ode78] = ode78(f, tspan, y0, options);
time_ode78 = toc;

tic;
[t_ode89, y_ode89] = ode89(f, tspan, y0, options);
time_ode89 = toc;

tic;
[t_ode113, y_ode113] = ode113(f, tspan, y0, options);
time_ode113 = toc;

% Výpočet chyb vůči analytickému řešení - použijeme y₅ pro A0 a y₂ pro A2
numerical_solution_taylor = [y_taylor(5,end); y_taylor(2,end)];
numerical_solution_euler = [y_euler(5,end); y_euler(2,end)];
numerical_solution_rk4 = [y_rk4(5,end); y_rk4(2,end)];
numerical_solution_ode23 = [y_ode23(end,5); y_ode23(end,2)];
numerical_solution_ode45 = [y_ode45(end,5); y_ode45(end,2)];
numerical_solution_ode78 = [y_ode78(end,5); y_ode78(end,2)];
numerical_solution_ode89 = [y_ode89(end,5); y_ode89(end,2)];
numerical_solution_ode113 = [y_ode113(end,5); y_ode113(end,2)];

% Debug výpis pro kontrolu hodnot
fprintf('\nKontrolní hodnoty:\n');
fprintf('Analytické řešení: A0 = %.6f, A2 = %.6f\n', A0, A2);
fprintf('Taylor: a0 = %.6f, a2 = %.6f\n', numerical_solution_taylor(1), numerical_solution_taylor(2));
fprintf('Euler: a0 = %.6f, a2 = %.6f\n', numerical_solution_euler(1), numerical_solution_euler(2));
fprintf('RK4: a0 = %.6f, a2 = %.6f\n', numerical_solution_rk4(1), numerical_solution_rk4(2));
fprintf('ODE23: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode23(1), numerical_solution_ode23(2));
fprintf('ODE45: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode45(1), numerical_solution_ode45(2));
fprintf('ODE78: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode78(1), numerical_solution_ode78(2));
fprintf('ODE89: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode89(1), numerical_solution_ode89(2));
fprintf('ODE113: a0 = %.6f, a2 = %.6f\n', numerical_solution_ode113(1), numerical_solution_ode113(2));

% Výpočet chyb
error_taylor = norm(numerical_solution_taylor - analytical_solution);
error_euler = norm(numerical_solution_euler - analytical_solution);
error_rk4 = norm(numerical_solution_rk4 - analytical_solution);
error_ode23 = norm(numerical_solution_ode23 - analytical_solution);
error_ode45 = norm(numerical_solution_ode45 - analytical_solution);
error_ode78 = norm(numerical_solution_ode78 - analytical_solution);
error_ode89 = norm(numerical_solution_ode89 - analytical_solution);
error_ode113 = norm(numerical_solution_ode113 - analytical_solution);

% Výpočet poměrů času
ratio_euler = time_euler/time_taylor;
ratio_rk4 = time_rk4/time_taylor;
ratio_ode23 = time_ode23/time_taylor;
ratio_ode45 = time_ode45/time_taylor;
ratio_ode78 = time_ode78/time_taylor;
ratio_ode89 = time_ode89/time_taylor;
ratio_ode113 = time_ode113/time_taylor;

% Počet kroků
steps_taylor = length(t_taylor);
steps_euler = length(t_euler);
steps_rk4 = length(t_rk4);
steps_ode23 = length(t_ode23);
steps_ode45 = length(t_ode45);
steps_ode78 = length(t_ode78);
steps_ode89 = length(t_ode89);
steps_ode113 = length(t_ode113);

% Vytvoření tabulky výsledků
fprintf('\nResults of calculations for Fourier coefficients:\n');
fprintf('--------------------------------------------------------\n');
fprintf('Solver\t\tTime [s]\t\tRatio\t\t||error||\tSteps\n');
fprintf('--------------------------------------------------------\n');
fprintf('Taylor\t\t%.5e\t%.1f\t\t%.2e\t%d\n', time_taylor, 1.0, error_taylor, steps_taylor);
fprintf('Euler\t\t%.5e\t%.1f\t\t%.2e\t%d\n', time_euler, ratio_euler, error_euler, steps_euler);
fprintf('RK4\t\t%.5e\t%.1f\t\t%.2e\t%d\n', time_rk4, ratio_rk4, error_rk4, steps_rk4);
fprintf('ode23\t\t%.5e\t%.1f\t\t%.2e\t%d\n', time_ode23, ratio_ode23, error_ode23, steps_ode23);
fprintf('ode45\t\t%.5e\t%.1f\t\t%.2e\t%d\n', time_ode45, ratio_ode45, error_ode45, steps_ode45);
fprintf('ode78\t\t%.5e\t%.1f\t\t%.2e\t%d\n', time_ode78, ratio_ode78, error_ode78, steps_ode78);
fprintf('ode89\t\t%.5e\t%.1f\t\t%.2e\t%d\n', time_ode89, ratio_ode89, error_ode89, steps_ode89);
fprintf('ode113\t\t%.5e\t%.1f\t\t%.2e\t%d\n', time_ode113, ratio_ode113, error_ode113, steps_ode113);