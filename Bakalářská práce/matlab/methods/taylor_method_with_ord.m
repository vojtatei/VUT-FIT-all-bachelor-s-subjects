function [t, y_taylor, ord] = taylor_method_with_ord(A, b, y0, h, t_final, EPS)
    % Inicializace času a vektorů pro řešení
    t = 0:h:t_final;
    num_steps = length(t);
    y_taylor = zeros(length(y0), num_steps);
    y_taylor(:, 1) = y0;
    maxTerms = 60;
    
    ord = zeros(1, num_steps);
    
    % Výpočet pomocí Taylorovy metody
    for i = 1:num_steps-1
        Tay_sum = y_taylor(:, i); % Počáteční člen y0
        term = h*(A*y_taylor(:, i) + b);
        j = 1;
        
        while true
            Tay_sum = Tay_sum + term;
            
            if (norm(term, Inf)) < EPS
                break;
            end
            
            % Aktualizace hodnot pro další iteraci
            j = j + 1;
            term = (h / j) * (A*term + b);
            
            if j > maxTerms
                warning('Taylorův krok %d potřebuje více než %d členů, zastavení.', i, maxTerms);
                break;
            end
        end
        
        ord(i+1) = j;
        
        % Aktualizace hodnoty y v dalším kroku
        y_taylor(:, i+1) = Tay_sum;
    end
end