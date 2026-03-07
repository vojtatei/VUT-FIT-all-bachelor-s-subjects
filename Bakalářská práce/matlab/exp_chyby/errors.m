addpath('../methods/')

% Řešení lineárního systému pomocí Taylorovy metody
% Definice parametrů
A = [0 1; -1 0];
b = [0; 0];           
y0 = [0; 5];          % Počáteční podmínky
t_final = 6;          % Konečný čas
EPS = 1e-12;          % Tolerance přesnosti

% Velikosti kroků pro analýzu
h_values = [0.01, 1];

% Analytické řešení
analytical_solution = @(t) [5*sin(t); 5*cos(t)];

% Uchování dat pro grafy
error_data = {};
ORD_data = {};

for idx = 1:length(h_values)
    h = h_values(idx); % Velikost kroku
    
    % Taylorova metoda
    [t_out, y_taylor, ord] = taylor_method_with_ord(A, b, y0, h, t_final, EPS);

    % Výpočet chyby
    y_exact = analytical_solution(t_out);
    error = vecnorm(y_taylor - y_exact, 2, 1); % Absolutní chyba (2-norma)
    
    % Výpočet relativní chyby
    denom = vecnorm(y_exact, 2, 1);
    rel_error = error ./ max(denom, 1e-10); % Prevence dělení nulou
    
    % Uložení dat
    error_data{idx} = rel_error;
    ORD_data{idx} = ord;
end

% Nastavení parametrů pro grafy
fs = 14; % Velikost písma
lw = 2;  % Tloušťka čáry
ms = 18;  % Velikost značek

% Graf 1: Funkce chyby pro h = 0.01
figure('Position', [100, 100, 600, 500]);
plot(0:h_values(1):t_final, error_data{1}, 'b.', 'MarkerSize', ms+4);
title('Funkce chyby pro h = 0.01', 'FontSize', fs+8);
xlabel('Čas [s]', 'FontSize', fs+8);
ylabel('Chyba', 'FontSize', fs+8);
set(gca, 'FontSize', fs+8);
grid on;

% Graf 2: ORD funkce pro h = 0.01
figure('Position', [700, 100, 600, 500]);
plot(0:h_values(1):t_final, ORD_data{1}, 'r.', 'MarkerSize', ms+4);
title('ORD funkce pro h = 0.01', 'FontSize', fs+8);
xlabel('Čas [s]', 'FontSize', fs+8);
ylabel('ORD', 'FontSize', fs+8);
set(gca, 'FontSize', fs+8);
grid on;

% Graf 3: Funkce chyby pro h = 1
figure('Position', [100, 650, 600, 500]);
plot(0:h_values(2):t_final, error_data{2}, 'b.', 'MarkerSize', ms+4);
title('Funkce chyby pro h = 1', 'FontSize', fs+2);
xlabel('Čas [s]', 'FontSize', fs);
ylabel('Chyba', 'FontSize', fs);
set(gca, 'FontSize', fs);
grid on;

% Graf 4: ORD funkce pro h = 1
figure('Position', [700, 650, 600, 500]);
plot(0:h_values(2):t_final, ORD_data{2}, 'r.', 'MarkerSize', ms+4);
title('ORD funkce pro h = 1', 'FontSize', fs+2);
xlabel('Čas [s]', 'FontSize', fs);
ylabel('ORD', 'FontSize', fs);
set(gca, 'FontSize', fs);
grid on;

% Graf 5: Analytické řešení
figure('Position', [1300, 100, 600, 500]);
t = 0:0.01:t_final;
y_exact_fine = analytical_solution(t);

plot(t, y_exact_fine(1,:), 'r-', t, y_exact_fine(2,:), 'b-', 'LineWidth', lw);
legend('y = 5*sin(t)', 'z = 5*cos(t)', 'FontSize', fs);
title('Analytické řešení', 'FontSize', fs+2);
xlabel('Čas [s]', 'FontSize', fs);
ylabel('Hodnota', 'FontSize', fs);
set(gca, 'FontSize', fs);
grid on;