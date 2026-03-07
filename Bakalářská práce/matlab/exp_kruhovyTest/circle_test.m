addpath('../methods/'); 

% Definice parametrů
b = [0; 0];
y0 = [0; 1];      % Počáteční podmínky
t_final = 50;     % Konečný čas
h = 0.01;          % Krok
omega = 100;      % Frekvence
A = [0 omega; -omega 0];

% Analytické řešení: sinus / cosinus
time_vector = 0:h:t_final;
y_analytical = [sin(time_vector); cos(time_vector)];

% Sestavení seznamu všech metod a jejich názvů
solvers = {
    @(A,b,y0,tf,h) taylor_method(A,b,y0,h,tf,1e-6)
    @(A,b,y0,tf,h) euler_method(A,b,y0,h,tf)
    @(A,b,y0,tf,h) rk4_method(A,b,y0,h,tf)
    @(A,b,y0,tf,h) builtin_ode(@ode23,  A,b,y0,tf)
    @(A,b,y0,tf,h) builtin_ode(@ode45,  A,b,y0,tf)
    @(A,b,y0,tf,h) builtin_ode(@ode78,  A,b,y0,tf)
    @(A,b,y0,tf,h) builtin_ode(@ode89,  A,b,y0,tf)
    @(A,b,y0,tf,h) builtin_ode(@ode113, A,b,y0,tf)
};

solver_names = {
    'Taylor','Euler','RK4','ode23','ode45','ode78','ode89','ode113'
};

% Smyčka: zavoláme každou metodu, uložíme výsledky
circle_data = cell(size(solvers));
for i = 1:length(solvers)
    method_handle = solvers{i};
    [t, y] = method_handle(A, b, y0, t_final, h);
    circle_data{i} = y;  
end

% Vykreslení analytického řešení
figure;
plot(y_analytical(1,:), y_analytical(2,:), 'k-', 'LineWidth',2);
title('Analytické řešení');
xlabel('y');
ylabel('z');
axis tight;
grid on;

% Vykreslení "kruhových" testů pro všechny metody 
num_plots_per_figure = 4;
num_figures = ceil(length(circle_data)/num_plots_per_figure);

for fig = 1:num_figures
    figure;
    start_idx = (fig-1)*num_plots_per_figure + 1;
    end_idx   = min(fig*num_plots_per_figure, length(circle_data));

    for subplot_idx = start_idx:end_idx
        subplot(2, 2, subplot_idx - start_idx +1);
        yvals = circle_data{subplot_idx};
        plot(yvals(1,:), yvals(2,:), 'o');
        title(['Kruhový test pro ', solver_names{subplot_idx}]);
        xlabel('y');
        ylabel('z');
        axis tight;
        grid on;
    end
    sgtitle(['Kruhové testy (Obrázek ', num2str(fig), ')']);
end

% Pomocná funkce pro volání vestavěných ODE solverů
function [tout, yout] = builtin_ode(odefunc, A,b,y0,t_final)
    fun = @(t,y) A*y + b;  
    opts = odeset('RelTol', 1e-3, 'AbsTol', 1e-3);
    [tout, ymat] = odefunc(fun, [0 t_final], y0, opts);
    tout = tout';  
    yout = ymat';  
end
