function [t, y_euler] = euler_method(A, b, y0, h, t_final, ~, ~)
% EULER_METHOD - lineární ODE s fixním krokem

    t = 0:h:t_final;
    num_steps = length(t);

    y_euler = zeros(length(y0), num_steps);
    y_euler(:,1) = y0;

    for i = 1:num_steps-1

        y_euler(:, i+1) = y_euler(:, i) + h*(A*y_euler(:, i) + b);
    end
end
