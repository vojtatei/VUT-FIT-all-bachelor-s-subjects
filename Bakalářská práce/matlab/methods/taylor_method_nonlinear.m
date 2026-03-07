function [t, y_taylor, ord] = taylor_method_nonlinear(A, B1, b, y_ij, y0, h, t_final, EPS, maxTerms)

    maxTerms = 60;


    % Inicializace času a vektorů pro řešení
    t = 0:h:t_final;
    num_steps = length(t);
    
    % Inicializace výsledku
    dim = length(y0);
    y_taylor = zeros(dim, num_steps);
    y_taylor(:, 1) = y0;
    
    % Inicializace vektoru ORD (počet členů Taylorovy řady v každém kroku)
    ord = zeros(1, num_steps);
    
    % Extrakce indexů pro kvadratické členy
    i_indices = y_ij(:, 1);
    j_indices = y_ij(:, 2);
    
    % Hlavní smyčka integrace
    for step = 1:num_steps-1
        % Aktuální hodnota y
        y_curr = y_taylor(:, step);
        
        % Alokace pro DY (všechny derivace - Taylorův polynom)
        DY = zeros(dim, maxTerms+1);
        DY(:, 1) = y_curr;
        
        % První derivace: A*y + B1*(y_i*y_j) + b
        % Lineární část: A*y
        dy_linear = A * y_curr;
        
        % Kvadratické členy: B1 * (y_i * y_j)
        quad_products = y_curr(i_indices) .* y_curr(j_indices);
        dy_quad = B1 * quad_products;
        
        % Celková první derivace
        DY(:, 2) = h * (dy_linear + dy_quad + b);
        
        % Inicializace y_new pro Taylorovu řadu
        y_new = y_curr + DY(:, 2);
        
        % Výpočet vyšších derivací a členů Taylorovy řady
        
        n = 2; 
        while n < maxTerms
            % Lineární část
            lin_term = A * DY(:, n);
            
            % Kvadratická část
            prods = DY(i_indices, n:-1:1) .* DY(j_indices, 1:n);
            sum_prods = sum(prods, 2);
            quad_term = B1 * sum_prods;
            
            % Nový člen Taylorovy řady
            DY(:, n+1) = (h/n) * (lin_term + quad_term);
            
            % Přidání nového členu do sumy
            y_new = y_new + DY(:, n+1);
            
            % Kontrola ukončení - pokud je člen dostatečně malý
            if norm(DY(:, n+1), Inf) < EPS
                break;
            end
            
            % Přechod na další řád
            n = n + 1;
        end
        
        % Uložení výsledku a počtu použitých členů
        y_taylor(:, step+1) = y_new;
        ord(step+1) = n;
    end
end