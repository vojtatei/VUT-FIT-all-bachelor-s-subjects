addpath('../methods/')
A = [0 1; -1 0];
b = [0; 0];
y0 = [0; 1];
h = 0.05;
t_final = 20;
EPS = 1e-6;

[t, y_sol] = taylor_method(A, b, y0, h, t_final, EPS);

plot(t, y_sol(1,:), 'r', t, y_sol(2,:), 'b');
title('Test sinus/cosinus');
xlabel('t');
ylabel('y(t)');
legend('y_1(t)','y_2(t)');
grid on;
