clc; clear; close all;

% 时间参数
dt = 0.1;
T = 10;
N = T/dt;
t = 0:dt:T-dt;

% 小车结构参数（真实模型使用）
L_f = 0.5; L_r = 0.5;  % 前后轴距

% 初始状态
x0 = [0; 0; 0]; % [x, y, theta]
v = 1.0;        % 恒定线速度
omega = 0.4;    % 恒定角速度

% 控制输入（用于理想模型）
u = [v; omega];

% 存储轨迹
x_ideal = zeros(3, N);
x_real = zeros(3, N);

x_ideal(:,1) = x0;
x_real(:,1) = x0;

for k = 1:N-1
    %% 理想模型（无侧滑）
    theta = x_ideal(3,k);
    x_ideal(1,k+1) = x_ideal(1,k) + dt * v * cos(theta);
    x_ideal(2,k+1) = x_ideal(2,k) + dt * v * sin(theta);
    x_ideal(3,k+1) = x_ideal(3,k) + dt * omega;

    %% 实际模型（带侧滑）
    psi = x_real(3,k);

    % 通过 omega 推测转角 δ： ω ≈ v/L * tan(δ) ⇒ δ ≈ atan(ω*L / v)
    L = L_f + L_r;
    delta = atan(omega * L / v);

    % 侧滑角 beta
    beta = atan((L_r / L) * tan(delta));

    % 更新状态
    x_real(1,k+1) = x_real(1,k) + dt * v * cos(psi + beta);
    x_real(2,k+1) = x_real(2,k) + dt * v * sin(psi + beta);
    x_real(3,k+1) = x_real(3,k) + dt * (v / L_r) * sin(beta);
end

%% 绘图对比
figure;
plot(x_ideal(1,:), x_ideal(2,:), 'b-', 'LineWidth', 2); hold on;
plot(x_real(1,:), x_real(2,:), 'r--', 'LineWidth', 2);
legend('理想轨迹（无侧滑）','实际轨迹（带侧滑）');
xlabel('X (m)'); ylabel('Y (m)');
title('理想轨迹 vs 实际轨迹（存在侧滑）');
grid on; axis equal;

% 误差图
figure;
plot(t, vecnorm(x_ideal(1:2,:) - x_real(1:2,:)), 'k', 'LineWidth', 1.5);
xlabel('时间 (s)'); ylabel('位置误差 (m)');
title('轨迹位置误差随时间（侧滑效应）');
grid on;
