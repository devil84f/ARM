function wmr_asosmc_neso_sim()
    clc; clear; close all;
    % 仿真参数
    Ts = 0.01; T_total = 20; N = T_total / Ts;

    % WMR 物理参数
    r = 0.05; b = 0.15;
    m = 3; Ic = 0.0675; Iw = 0.01125;

    % 控制器参数
    k1 = 14; k2 = 1; alpha = 7;
    beta1 = 50; beta2 = 300;
    j = 13; p = 7; psi = 30; lambda = 0.2;

    % 初始状态
    q = [0; 0; 0];               % 位姿 [x; y; theta]
    zeta = [0; 0];              % [v; omega]
    zeta_hat = [0; 0];          % NESO估计
    D_hat = [0; 0];             % NESO估计的扰动
    gamma_hat = [0; 0];         % 自适应估计
    S_int = [0; 0];             % 滑模积分

    % 数据记录
    Q = zeros(3, N); E = zeros(2, N); ZETA = zeros(2, N);

    % 主循环
    for k = 1:N
        t = k * Ts;

        % 期望速度轨迹（直线前进 + 恒转弯）
        zeta_d = [1.0; 0.2];
        zeta_d_dot = [0; 0];

        % 扰动（外界扰动模拟）
        D = [0.5 + sin(2*t); 0.5 + cos(2*t)];

        % NESO估计误差
        e1 = zeta - zeta_hat;
        fal_e = fal(e1, 0.5, 0.01);

        % NESO更新
        zeta_hat = zeta_hat + Ts * (D_hat + beta1 * fal_e);
        D_hat = D_hat + Ts * (beta2 * fal_e);

        % 滑模变量
        e = zeta_d - zeta;
        sigma = zeta_d_dot - D_hat + k1 * e + k2 * (e.^(j/p));
        S = sigma + alpha * S_int;
        S_int = S_int + Ts * sigma;

        % 自适应律
        gamma_hat = gamma_hat + Ts * (lambda * abs(S));

        % 控制律计算
        tau_desired = zeta_d_dot + k1*e + k2*(e.^(j/p)) + alpha*sigma + psi*S + gamma_hat .* sign(S);
        Gamma = [r, r; -r*b, r*b];
        tau = pinv(Gamma) * tau_desired;

        % 动力学积分
        M_eq = [m*r^2 + 2*Iw, 0; 0, r^2*Ic + 2*b^2*Iw];
        zeta_dot = M_eq \ (Gamma * tau + D);
        zeta = zeta + Ts * zeta_dot;

        % 运动学积分
        theta = q(3);
        J = [cos(theta), 0; sin(theta), 0; 0, 1];
        q = q + Ts * (J * zeta);

        % 数据记录
        Q(:, k) = q; E(:, k) = e; ZETA(:, k) = zeta;
    end

    %% 绘图
    figure;
    plot(Q(1, :), Q(2, :), 'b', 'LineWidth', 2); hold on;
    quiver(Q(1, 1:100:end), Q(2, 1:100:end), cos(Q(3, 1:100:end)), sin(Q(3, 1:100:end)), 0.5, 'r');
    title('WMR 轨迹'); xlabel('X [m]'); ylabel('Y [m]');
    axis equal; grid on;

    figure;
    plot(E(1, :), 'r'); hold on; plot(E(2, :), 'g');
    title('速度误差'); legend('v误差', '\omega误差'); grid on;

    figure;
    plot(ZETA(1, :), 'r'); hold on; plot(ZETA(2, :), 'g');
    title('速度响应'); legend('v', '\omega'); grid on;
end

% 非线性函数 fal(e, ε, σ)
function y = fal(e, eps, sig)
    y = (abs(e) > sig) .* (abs(e).^eps .* sign(e)) + ...
        (abs(e) <= sig) .* (e / sig);
end
