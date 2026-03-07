addpath('../methods/');
% Definice problému y' = -2y, y(0) = 1
f = @(t, y) -2*y;
y_exact = @(t) exp(-2*t);  % Analytické řešení y(t) = e^(-2t)
t_end = 5;
y0 = 1;

% Různé velikosti kroku
h_values = 2.^(-1:-1:-10); 
errors_euler = zeros(size(h_values));
errors_rk4 = zeros(size(h_values));

for i = 1:length(h_values)
    h = h_values(i);
    
    % Euler
    [t_euler, y_euler] = euler_method(f, [0, t_end], y0, h);
    errors_euler(i) = abs(y_euler(end) - y_exact(t_end));
    
    % RK4
    [t_rk4, y_rk4] = rk4_method(f, [0, t_end], y0, h);
    errors_rk4(i) = abs(y_rk4(end) - y_exact(t_end));
end

% Logaritmický graf
figure('Position', [100, 100, 800, 600]);
loglog(h_values, errors_euler, 'ro-', 'LineWidth', 2);
hold on;
loglog(h_values, errors_rk4, 'bo-', 'LineWidth', 2);

% Referenční přímky pro sklon 1 a 4
ref_slope1 = h_values .* (errors_euler(5)/h_values(5));
ref_slope4 = h_values.^4 .* (errors_rk4(5)/h_values(5)^4);
loglog(h_values, ref_slope1, 'r--', 'LineWidth', 1.5);
loglog(h_values, ref_slope4, 'b--', 'LineWidth', 1.5);

grid on;
xlabel('Velikost kroku h', 'FontSize', 30);
ylabel('Globální chyba', 'FontSize', 30);
title('Závislost globální chyby na velikosti kroku', 'FontSize', 40);


legend('Eulerova metoda (p=1)', 'RK4 metoda (p=4)', 'Sklon 1', 'Sklon 4', ...
    'Location', 'southeast', 'FontSize', 20);


ax = gca;
ax.FontSize = 16;

% Funkce pro Eulerovu metodu
function [t, y] = euler_method(f, tspan, y0, h)
    t = tspan(1):h:tspan(2);
    y = zeros(size(t));
    y(1) = y0;
    
    for i = 1:length(t)-1
        y(i+1) = y(i) + h * f(t(i), y(i));
    end
end

% Funkce pro metodu RK4
function [t, y] = rk4_method(f, tspan, y0, h)
    t = tspan(1):h:tspan(2);
    y = zeros(size(t));
    y(1) = y0;
    
    for i = 1:length(t)-1
        k1 = f(t(i), y(i));
        k2 = f(t(i) + h/2, y(i) + h*k1/2);
        k3 = f(t(i) + h/2, y(i) + h*k2/2);
        k4 = f(t(i) + h, y(i) + h*k3);
        
        y(i+1) = y(i) + h * (k1 + 2*k2 + 2*k3 + k4) / 6;
    end
end