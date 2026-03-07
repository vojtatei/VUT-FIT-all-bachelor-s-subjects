function [t, y_rk4] = rk4_method(A, b, y0, h, t_final, ~, ~)
% RK4_METHOD - 4. řád Runge-Kutta

    t = 0:h:t_final;
    num_steps = length(t);

    y_rk4 = zeros(length(y0), num_steps);
    y_rk4(:,1) = y0;

    for i = 1:num_steps-1
        y_cur = y_rk4(:, i);

        if isa(b, 'function_handle')
            b1 = b(t(i));
            b2 = b(t(i) + 0.5*h);
            b3 = b(t(i) + 0.5*h);
            b4 = b(t(i) + h);
        else
            b1 = b; b2 = b; b3 = b; b4 = b;
        end

        k1 = A*y_cur + b1;
        k2 = A*(y_cur + 0.5*h*k1) + b2;
        k3 = A*(y_cur + 0.5*h*k2) + b3;
        k4 = A*(y_cur + h*k3)     + b4;

        y_rk4(:, i+1) = y_cur + (h/6)*(k1 + 2*k2 + 2*k3 + k4);
    end
end
