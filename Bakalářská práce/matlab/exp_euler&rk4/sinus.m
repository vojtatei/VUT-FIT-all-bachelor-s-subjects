addpath('../methods/')
% Definice parametrů
A = [0 1; -1 0];
b = [0; 0];
y0 = [0; 1];
h = 0.05;
t_final = 20;

% Řešení pomocí Eulerovy metody
[~, y_euler] = euler_method(A, b, y0, h, t_final); 

% Řešení pomocí RK4 metody
[~, y_rk4]   = rk4_method(A, b, y0, h, t_final); 

% Příprava časového vektoru pro vykreslení
t = 0:h:t_final;

% Vykreslení výsledků Eulerovy metody
figure;
plot(t, y_euler(1,:), 'r', t, y_euler(2,:), 'b');
title('Řešení pomocí Eulerovy metody');
legend('y_1(t)', 'y_2(t)');
xlabel('t');
ylabel('y(t)');
grid on;

% Vykreslení výsledků RK4 metody
figure;
plot(t, y_rk4(1,:), 'r', t, y_rk4(2,:), 'b');
title('Řešení pomocí RK4 metody');
legend('y_1(t)','y_2(t)');
xlabel('t');
ylabel('y(t)');
grid on;
