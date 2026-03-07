
close all; clear; clc;
addpath('../methods/')
% Definice parametrů Lorenzova systému
sigma = 10;          % Prandtlovo číslo
beta = 8/3;          % Parametr související s fyzikální velikostí systému

% Hodnoty parametru rho pro různé režimy
rho_values = [28, 160, 23.7];


target_steps = [2000, 4000, 500];  

% Nastavení integrace
t_final = 100;     
EPS = 1e-10;       

% Nastavení tolerance pro ODE solvery
options = odeset('RelTol', 1e-10, 'AbsTol', 1e-10); 


% Simulace pro každou hodnotu rho
for idx = 1:length(rho_values)
    rho = rho_values(idx);

    if rho == 23.7
        h_plot = 0.001; 
    else
        h_plot = 0.005; 
    end
    
    % Krok pro měření
    steps = target_steps(idx);
    h_calc = t_final / steps;
    
    
    % Vytvoření lineární části matice A pro aktuální rho
    A = [-sigma, sigma, 0;
          rho,    -1,   0;
           0,      0, -beta];
    
    % Matice B1 pro kvadratické členy
    B1 = [0, 0;
         -1, 0;
          0, 1];
          
    % Indexy y_ij
    y_ij = [1, 3;   
            1, 2];   
            
    % Konstantní člen
    b = [0; 0; 0];
    
    % Q± = (±√β(ρ-1), ±√β(ρ-1), ρ-1)
    v = [0; 2; 0]; 
    
    if rho == 23.7
        eq_point = [sqrt(beta*(rho-1)); sqrt(beta*(rho-1)); rho-1];
        y0 = eq_point + v; 
    else
    
        y0 = [1; 1; 1];
    end
    
    % Simulace s jemným krokem pro vykreslování
    [t_plot, y_plot, ord_plot] = taylor_method_nonlinear(A, B1, b, y_ij, y0, h_plot, t_final, EPS);
    
    % Simulace s krokem pro měření a porovnání
    tic;
    [t, y, ord] = taylor_method_nonlinear(A, B1, b, y_ij, y0, h_calc, t_final, EPS);
    mtsm_time = toc;
    
    % Počet kroků MTSM
    mtsm_steps = length(t) - 1;
    
    plot_indices = 1:length(t_plot);
    
    figure('Position', [100, 100, 800, 600]);
    
    if rho == 23.7
        plot(y_plot(2,plot_indices), y_plot(3,plot_indices), '-', 'LineWidth', 0.2, 'Color', [0, 0, 1, 0.2]);
        hold on;

        eq_point = [sqrt(beta*(rho-1)); sqrt(beta*(rho-1)); rho-1];
        plot(eq_point(2), eq_point(3), 'w.', 'MarkerSize', 10);
    else

        plot(y_plot(2,plot_indices), y_plot(3,plot_indices), 'b-', 'LineWidth', 0.3);
    end
    
    title(sprintf('Lorenzův systém v yz-rovině, \\rho = %g', rho), 'FontSize', 14);
    xlabel('y', 'FontSize', 12);
    ylabel('z', 'FontSize', 12);
    
    % Nastavení rozsahu os podle režimu
    if rho == 28
        xlim([-25, 25]);
        ylim([0, 50]);
    elseif rho == 160
        xlim([-100, 100]);
        ylim([100, 220]);
    else  % rho = 23.7
        xlim([6, 9.5]);
        ylim([20.5, 25]);
    end
    
    grid on;
    
    % Vykreslení časových průběhů x, y, z
    figure('Position', [150, 150, 800, 600]);
    
    subplot(3,1,1);
    plot(t_plot(plot_indices), y_plot(1,plot_indices), 'b-', 'LineWidth', 0.5);
    title('x(t)', 'FontSize', 12);
    ylabel('x', 'FontSize', 11);
    grid on;
    ylim([-50 50]);
    xlim([0 100]);
    
    subplot(3,1,2);
    plot(t_plot(plot_indices), y_plot(2,plot_indices), 'b-', 'LineWidth', 0.5);
    title('y(t)', 'FontSize', 12);
    ylabel('y', 'FontSize', 11);
    grid on;
    ylim([-50 50]);
    xlim([0 100]);
    
    subplot(3,1,3);
    plot(t_plot(plot_indices), y_plot(3,plot_indices), 'b-', 'LineWidth', 0.5);
    title('z(t)', 'FontSize', 12);
    xlabel('t', 'FontSize', 11);
    ylabel('z', 'FontSize', 11);
    grid on;
    ylim([-50 50]);
    xlim([0 100]);
    
    % Graf funkce ORD - červené tečky
    figure('Position', [200, 200, 800, 600]);
    plot(t(1:length(ord)), ord, 'r.', 'MarkerSize', 4);
    title(sprintf('Funkce ORD pro \\rho = %g', rho), 'FontSize', 14);
    xlabel('t', 'FontSize', 12);
    ylabel('ORD', 'FontSize', 12);
    grid on;
    ylim([0 60]);  
    
    % Definice Lorenzova systému pro ODE solvers
    lorenz = @(t, y) [
        sigma * (y(2) - y(1));                % x'
        rho * y(1) - y(2) - y(1) * y(3);      % y'
        y(1) * y(2) - beta * y(3)             % z'
    ];
    
    % ODE23
    tic;
    [t_ode23, y_ode23] = ode23(lorenz, [0, t_final], y0, options);
    ode23_time = toc;
    ode23_steps = length(t_ode23) - 1;
    ode23_ratio = ode23_time / mtsm_time;
    
    % ODE45
    tic;
    [t_ode45, y_ode45] = ode45(lorenz, [0, t_final], y0, options);
    ode45_time = toc;
    ode45_steps = length(t_ode45) - 1;
    ode45_ratio = ode45_time / mtsm_time;
    
    % ODE78
    tic;
    [t_ode78, y_ode78] = ode78(lorenz, [0, t_final], y0, options);
    ode78_time = toc;
    ode78_steps = length(t_ode78) - 1;
    ode78_ratio = ode78_time / mtsm_time;
    
    % ODE89
    tic;
    [t_ode89, y_ode89] = ode89(lorenz, [0, t_final], y0, options);
    ode89_time = toc;
    ode89_steps = length(t_ode89) - 1;
    ode89_ratio = ode89_time / mtsm_time;
    
    % ODE113
    tic;
    [t_ode113, y_ode113] = ode113(lorenz, [0, t_final], y0, options);
    ode113_time = toc;
    ode113_steps = length(t_ode113) - 1;
    ode113_ratio = ode113_time / mtsm_time;

    vysledky = [];
    vysledky = [vysledky; {'Taylor', mtsm_steps, mtsm_time, 1}];
    vysledky = [vysledky; {'ode23', ode23_steps, ode23_time, ode23_ratio}];
    vysledky = [vysledky; {'ode45', ode45_steps, ode45_time, ode45_ratio}];
    vysledky = [vysledky; {'ode78', ode78_steps, ode78_time, ode78_ratio}];
    vysledky = [vysledky; {'ode89', ode89_steps, ode89_time, ode89_ratio}];
    vysledky = [vysledky; {'ode113', ode113_steps, ode113_time, ode113_ratio}];
    
   
    fprintf('\nVýsledky pro rho = %g:\n\n', rho);
    fprintf('%-10s %-12s %-12s %-8s\n', 'Metoda', 'PocetKroku', 'CasVypoctu', 'Pomer');
    fprintf('----------- ------------ ------------ --------\n');
    
    % Tisk tabulky v konzoli
    for i = 1:size(vysledky, 1)
        fprintf('%-10s %-12d %-.6g %-8.5g\n', ...
                sprintf('{''%s''}', vysledky{i, 1}), vysledky{i, 2}, vysledky{i, 3}, vysledky{i, 4});
    end
    
end
