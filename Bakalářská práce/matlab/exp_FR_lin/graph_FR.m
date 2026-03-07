addpath('../methods/'); 

% Parametry
T = 2;                  % Perioda
omega = 2 * pi / T;     % Úhlová frekvence
tspan = [0 T];          % Interval pro řešení
y0 = [0; 0];           % Počáteční podmínky pro a0 a a2

% Definice diferenciálních rovnic
odefun = @(t, y) [sin(omega * t).^2; 
                  (2/T) * sin(omega * t).^2 .* cos(2 * omega * t)];

% Řešení pomocí ode45
[t, y] = ode45(odefun, tspan, y0);

% Extrahování výsledků
a0 = y(:,1);
a2 = y(:,2);

% Vykreslení výsledků
figure;
hold on;
plot(t, a0, 'b-', 'LineWidth', 1.5); % Modrá - a0
plot(t, a2, 'r--', 'LineWidth', 1.5); % Červená přerušovaná - a2
legend('a_0', 'a_2', 'Location', 'best');
xlabel('Čas [s]');
ylabel('Fourierovy koeficienty');
title('Fourierovy koeficienty pro f(t) = sin^2(\omega t)');
grid on;
hold off;
