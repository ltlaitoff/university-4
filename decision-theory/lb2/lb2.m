Mx = 4.0;
My = 20.0;
sig_x = 0.6;
sig_y = 1.2;
Dx = sig_x^2;
Dy = sig_y^2;
R1 = -0.5;
R2 = -0.8;
x1 = 5.0;
x2 = 4.0;
x3 = 3.0;
x4 = 2.0;
X_vals = [x1, x2, x3, x4];

% For graphs
x_range = linspace(Mx - 3*sig_x, Mx + 3*sig_x, 100);
y_range = linspace(My - 3*sig_y, My + 3*sig_y, 100);

% 2.2
Wx = (1/(sig_x*sqrt(2*pi))) * exp(-(x_range-Mx).^2 / (2*Dx));
Wy = (1/(sig_y*sqrt(2*pi))) * exp(-(y_range-My).^2 / (2*Dy));

[X, Y] = meshgrid(x_range, y_range);
A = 1 / (2*pi*sig_x*sig_y*sqrt(1-R1^2));
B = -1 / (2*(1-R1^2));
Z = ((X-Mx).^2/Dx) - (2*R1*(X-Mx).*(Y-My)/(sig_x*sig_y)) + ((Y-My).^2/Dy);
Wxy = A * exp(B * Z);

My_x = My + R1 * (sig_y/sig_x) * (x1 - Mx);
Dy_x = Dy * (1 - R1^2);
D_err = Dy_x;

Wy_x = (1/(sqrt(2*pi*Dy_x))) * exp(-(y_range-My_x).^2 / (2*Dy_x));

fprintf('--- Результати пункту 2.2 ---\n');
fprintf('1. Умовне мат. сподівання M[y/x] = %.4f\n', My_x);
fprintf('2. Умовна дисперсія D[y/x] = %.4f\n', Dy_x);
fprintf('3. Дисперсія похибки D[delta y] = %.4f\n', D_err);

% 2.3
Dy_base = sig_y^2;
r_range = 0:0.1:1.0;
Dy_vals = [Dy_base, 2*Dy_base, 4*Dy_base];
table_data = zeros(length(r_range), 3);

for j = 1:3
    for i = 1:length(r_range)
        % D_err = Dy * (1 - r^2)
        table_data(i, j) = Dy_vals(j) * (1 - r_range(i)^2);
    end
end

fprintf('\nТаблиця залежності D[delta y] від r та D[y]:\n');
fprintf('|  |r|  | D[y]1=%.2f | D[y]2=%.2f | D[y]3=%.2f |\n', Dy_vals(1), Dy_vals(2), Dy_vals(3));
fprintf('----------------------------------------------------------\n');
for i = 1:length(r_range)
    fprintf('|  %.1f  |   %.4f   |   %.4f   |   %.4f   |\n', ...
            r_range(i), table_data(i,1), table_data(i,2), table_data(i,3));
end

% 2.4
k_vals = [0, 0.2, 0.4, 0.6, 0.8, 1, 2, 3];
y_points = unique([My - flip(k_vals)*sig_y, My + k_vals*sig_y]);
Wy_points = (1/(sig_y*sqrt(2*pi))) * exp(-(y_points-My).^2 / (2*Dy));

fprintf('\n--- Результати пункту 2.4 (Значення W(y)) ---\n');
fprintf('    y    |   W(y)   \n');
fprintf('--------------------\n');
for i = 1:length(y_points)
    fprintf('%8.2f | %10.6f \n', y_points(i), Wy_points(i));
end

% 2.5
r_to_test = [R1, R2];
k_steps = [0, 0.2, 0.4, 0.6, 0.8, 1, 2, 3];

fprintf('\n--- Результати пункту 2.5 (Таблиця умовних густин) ---\n');

for idx_r = 1:2
    curr_r = r_to_test(idx_r);
    Dy_x = Dy * (1 - curr_r^2);
    sig_y_x = sqrt(Dy_x);

    fprintf('\nДля r = %.1f:\n', curr_r);
    fprintf(' к*sig |   X=5.0    |   X=4.0    |   X=3.0    |   X=2.0    |\n');
    fprintf('----------------------------------------------------------\n');

    for k = k_steps
        row_str = sprintf(' %3.1fσ  |', k);
        for idx_x = 1:4
            xj_curr = X_vals(idx_x);
            My_x_curr = My + curr_r * (sig_y/sig_x) * (xj_curr - Mx);

            y_point = My_x_curr + k * sig_y;

            W_val = (1/(sig_y_x*sqrt(2*pi))) * exp(-(y_point-My_x_curr).^2 / (2*Dy_x));
            row_str = [row_str, sprintf('  %.6f  |', W_val)];
        end

        fprintf('%s\n', row_str);
    end
end

% 2.7
My_x_theory = My + R1 * (sig_y/sig_x) * (x2 - Mx);
Dy_x_eval = Dy * (1 - R1^2);

target_func = @(y) -(1/(sqrt(2*pi*Dy_x_eval))) * exp(-(y - My_x_theory).^2 / (2*Dy_x_eval));

eps = 0.0001;
max_it = 100;

% Метод дихотомії

a1 = 15; b1 = 25;
iter_dich = 0;

while (b1 - a1) > eps && iter_dich < max_it
    iter_dich = iter_dich + 1;
    y_mid = (a1 + b1) / 2;
    f1 = target_func(y_mid - eps/10);
    f2 = target_func(y_mid + eps/10);

    if f1 < f2
        b1 = y_mid;
    else
        a1 = y_mid;
    end
end
y_opt_dich = (a1 + b1) / 2;

% Метод Пауелла
opts_p = optimset('TolX', eps, 'MaxIter', max_it);
[y_opt_pow, ~, ~, output_p] = fminbnd(target_func, 15, 25, opts_p);
iter_pow = output_p.iterations;

% Метод Ньютона
opts_n = optimset('TolX', eps, 'MaxIter', max_it, 'Display', 'off');
[y_opt_newt, ~, ~, output_n] = fminunc(target_func, 19, opts_n);
iter_newt = output_n.iterations;

fprintf('\n--- Результати пункту 2.3.7 ---\n');
fprintf('Метод             |  Результат Y* | Ітерацій\n');
fprintf('--------------------------------------------\n');
fprintf('Дихотомія         |   %.6f    |   %d\n', y_opt_dich, iter_dich);
fprintf('Пауелла           |   %.6f    |   %d\n', y_opt_pow, iter_pow);
fprintf('Ньютона-Рафсона   |   %.6f    |   %d\n', y_opt_newt, iter_newt);
fprintf('--------------------------------------------\n');
fprintf('Теоретична мода   |   %.6f\n', My_x_theory);

% --- 2.8
% M[y/x] = My + r * (sig_y / sig_x) * (x - Mx)
Y_opt_theory = My + R1 * (sig_y / sig_x) * (x2 - Mx);

fprintf('\n--- Результати пункту 2.8 ---\n');
fprintf('Оптимальна оцінка Y*(2) за формулою: %.4f\n', Y_opt_theory);

% 2.9
Y_ref = Y_opt_theory;

Y_results = [y_opt_dich, y_opt_pow, y_opt_newt];
method_names = {'Дихотомія', 'Пауелла', 'Ньютона-Рафсона'};

fprintf('\n--- Результати пункту 2.3.9 (Аналіз похибок) ---\n');
fprintf('Метод             | Абс. похибка | Відн. похибка (%%)\n');
fprintf('------------------------------------------------------\n');

for i = 1:3
    abs_err = abs(Y_results(i) - Y_ref);

    rel_err = (abs_err / Y_ref) * 100;

    fprintf('%-17s |   %.8f   |   %.8f %%\n', ...
            method_names{i}, abs_err, rel_err);
end

% --- Побудова графіків ---

% W(x,y)
figure(1);
surf(X, Y, Wxy);
shading interp; colorbar;
title('Спільна густина розподілу W(x,y)');
xlabel('x'); ylabel('y'); zlabel('W(x,y)');

% W(x), W(y), W(y/x)
figure(2);
subplot(3,1,1); plot(x_range, Wx, 'LineWidth', 2); grid on;
title('Одновимірна густина W(x)');
subplot(3,1,2); plot(y_range, Wy, 'LineWidth', 2); grid on;
title('Одновимірна густина W(y)');
subplot(3,1,3); plot(y_range, Wy_x, 'r', 'LineWidth', 2); grid on;
title(['Умовна густина W(y/x) при x = ', num2str(x1)]);
xlabel('y');

% 2.3
figure(3);
plot(r_range, table_data(:,1), '-ob', 'LineWidth', 1.5); hold on;
plot(r_range, table_data(:,2), '-sg', 'LineWidth', 1.5);
plot(r_range, table_data(:,3), '-^r', 'LineWidth', 1.5);
grid on;
legend(['D[y] = ', num2str(Dy_vals(1))], ...
       ['D[y] = ', num2str(Dy_vals(2))], ...
       ['D[y] = ', num2str(Dy_vals(3))]);
xlabel('Коефіцієнт кореляції |r|');
ylabel('Дисперсія похибки D[\Delta y]');
title('Залежність дисперсії похибки від |r| та D[y]');

% 2.4
figure(4);
plot(y_range, Wy, 'b', 'LineWidth', 1.5); hold on;
plot(y_points, Wy_points, 'ro', 'MarkerFaceColor', 'r');
grid on;
xlabel('Параметр y');
ylabel('Густина W(y)');
title('Безумовна густина W(y) з розрахованими точками');
legend('Крива W(y)', 'Розраховані точки');

% 2.6
colors = ['r', 'g', 'b', 'k'];
r_to_plot = [R1, R2];

for idx_r = 1:2
    figure('Name', ['Аналіз густин при r = ', num2str(r_to_plot(idx_r))]);
    hold on;

    plot(y_range, Wy, '--k', 'LineWidth', 2, 'DisplayName', 'Безумовна W(y)');

    curr_r = r_to_plot(idx_r);
    Dy_x = Dy * (1 - curr_r^2);

    for idx_x = 1:4
        xj_curr = X_vals(idx_x);
        My_x_curr = My + curr_r * (sig_y/sig_x) * (xj_curr - Mx);
        Wy_x_curr = (1/(sqrt(2*pi*Dy_x))) * exp(-(y_range-My_x_curr).^2 / (2*Dy_x));

        plot(y_range, Wy_x_curr, colors(idx_x), 'LineWidth', 1.5, ...
             'DisplayName', ['W(y/x) при X = ', num2str(xj_curr)]);
    end

    grid on;
    xlabel('Параметр Y'); ylabel('Густина');
    title(['Порівняльний аналіз густин (r = ', num2str(curr_r), ')']);
    legend('Location', 'northeastoutside');
end

