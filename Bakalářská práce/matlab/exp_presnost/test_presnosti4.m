addpath('../methods/');     

omega = 100;                   % Frekvence pro sin(t), cos(t)
A = [0 omega; -omega 0];             % Matice systému
b = [0; 0];                  % Vektor b
y0 = [0; 1];                 % Počáteční podmínka
t_final = 50;               

analytical_solution = @(t) [sin(omega*t); cos(omega*t)];

MTSMTOL = 1e-6;              % Pro Taylorovu metodu
MTSMh   = 0.01;               % Krok pro Taylor
TOL     = 1e-3;              % Tolerance pro vestavěné ODE
h_euler = 0.01;               % Krok pro Euler a RK4

solvers = {
    @taylor_method, ...
    @euler_method, ...
    @rk4_method,    ...
    'ode23', ...
    'ode45', ...
    'ode78', ...
    'ode89', ...
    'ode113'
};
solver_names = {
    'Taylor','Euler','RK4','ode23','ode45','ode78','ode89','ode113'
};

vysledky = [];
ref_cas  = [];

for i = 1:length(solvers)
    solverID    = solvers{i};
    solver_name = solver_names{i};

    if strcmp(solver_name,'Taylor')
        h = MTSMh;        % 0.1
    elseif strcmp(solver_name,'Euler') || strcmp(solver_name,'RK4')
        h = h_euler;      % 0.01
    else
        h = [];         
    end

    tic;
    if ischar(solverID)   % Vestavěný solver (odeXX)
        ode_fun = @(t,Y) A*Y + b;
        [tout, Ymat] = feval(solverID, ode_fun, [0 t_final], y0, ...
            odeset('RelTol', TOL, 'AbsTol', TOL));
        tout = tout';     
        y    = Ymat';     
        t    = tout;
    else                  % Vlastní metoda (Taylor/Euler/RK4)
        [t, y] = solverID(A, b, y0, h, t_final, MTSMTOL, TOL);
    end
    cas_vypoctu = toc;

    y_ana  = analytical_solution(t);
    rozdil = y - y_ana;
    chyba  = max(vecnorm(rozdil,2,1));
    pocet_kroku = length(t);

    if strcmp(solver_name,'Taylor')
        ref_cas = cas_vypoctu;
        pomer_casu = 1;
    else
        pomer_casu = cas_vypoctu / ref_cas;
    end

    vysledky = [vysledky; ...
        {solver_name, pocet_kroku, chyba, cas_vypoctu, pomer_casu}];
end

vysledky_tab = cell2table(vysledky, ...
   'VariableNames',{'Metoda','PocetKroku','Chyba','CasVypoctu','Pomer'});
disp(vysledky_tab);
