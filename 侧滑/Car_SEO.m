% 基于扩张状态观测器的小车控制
clc; clear all; close all;
set(0,'defaultfigurecolor','w');

span=0.2; % 仿真时长
dt=0.001; % 采样间隔
N=floor(span/dt)+1;
%% 建模
% 离散模型(1)
nx=5; % x的维度
x=zeros(nx,N); 
x(:,1) = [0, 0, 0, 0, 0];
A = [1 0 0 0 0;
     0 1 0 0 0;
     0 0 1 0 0;
     0 0 0 0 0;
     0 0 0 0 0];
B = [dt*cos(x(3,1)) 0;
     dt*sin(x(3,1)) 0;
     0                   dt;
     1                   0;
     0                   1];
C = [1 0 0 0 0;
     0 1 0 0 0;
     0 0 1 0 0;
     0 0 0 1 0;
     0 0 0 0 1];
% 增广(2)
Theta=[A, B; zeros(2, nx) eye(2)]; % 扩张状态
Delta_u=[B; zeros(2,2)];
Delta_h=[zeros(nx,2); eye(2)];
H=[C, zeros(nx,2)];

%% 观测器
po=[0.1 0.2 0.3 0.4 0.5 0.6 0.7]; % 极点
L=place(Theta', H', po)'; % 配置极点

%% 仿真
x_hat=zeros(nx,N);
d=zeros(2,N);
d_hat=zeros(2,N);
u=zeros(2,N);
y=zeros(nx,N); 
y(:, 1)=C*x(:,1);
h=0.1*dt^3*ones(2,N); % 误差随机小扰动
xi_hat=zeros(nx+2,N);

for k=1:N-1
    % 真实系统
    x(:,k+1)=A*x(:,k)+B*u(:, k)+B*d(:, k);
    B = [dt*cos(x(3,k)) 0;
     dt*sin(x(3,k)) 0;
     0                   dt;
     1                   0;
     0                   1]; % 更新B矩阵
    d(:, k+1)=d(:, k)+h(:, k);
    % 突发大扰动 或 扰动转换
    if k==floor(N/2)
        d(1, k+1)=d(1, k+1)+0.1;
        d(2, k+1)=d(2, k+1)+0.5;
    end
    y(:,k+1)=C*x(:,k+1);
    % 估计器
    xi_hat(:,k+1)=Theta*xi_hat(:, k) + Delta_u*u(:, k)+Delta_h*h(:, k)+L*(y(:, k)-H*xi_hat(:, k));
    x_hat(:,k+1)=xi_hat(1:nx, k+1); % 状态估计
    d_hat(:, k+1)=xi_hat(nx+1:end, k+1); % 误差估计
    % 更新SEO中B矩阵
    Theta=[A, B; zeros(2, nx) eye(2)];
    Delta_u=[B; zeros(2,2)];
    % 更新SEO中极点配置
    po=[0.1 0.2 0.3 0.4 0.5 0.6 0.7]; % 极点
    L=place(Theta', H', po)'; % 配置极点
    % 控制器

end

ex=x-x_hat;
ed=d-d_hat;

%% 画图（优化版）
% 设置统一的绘图风格
line_width = 1.5;
font_size = 11;

% 1. 干扰变化（真实 vs 估计）
figure('Name', '干扰变化', 'Position', [100, 100, 800, 400]);
subplot(2,1,1);
plot(0:dt:span, d(1,:), 'LineWidth', line_width, 'Color', 'b', 'DisplayName', '真实干扰 d_1');
hold on;
plot(0:dt:span, d_hat(1,:), '--', 'LineWidth', line_width, 'Color', 'r', 'DisplayName', '估计干扰d_1');
xlabel('时间 (s)', 'FontSize', font_size);
ylabel('干扰 d_1', 'FontSize', font_size);
title('干扰变化 (d_1)', 'FontSize', font_size+2);
grid on;
legend('show', 'Location', 'best');

subplot(2,1,2);
plot(0:dt:span, d(2,:), 'LineWidth', line_width, 'Color', 'b', 'DisplayName', '真实干扰 d_2');
hold on;
plot(0:dt:span, d_hat(2,:), '--', 'LineWidth', line_width, 'Color', 'r', 'DisplayName', '估计干扰d_2');
xlabel('时间 (s)', 'FontSize', font_size);
ylabel('干扰 d_2', 'FontSize', font_size);
title('干扰变化 (d_2)', 'FontSize', font_size+2);
grid on;
legend('show', 'Location', 'best');

% 2. 状态输出值（x, y, θ）
figure('Name', '状态输出值', 'Position', [100, 100, 800, 600]);
subplot(3,1,1);
plot(0:dt:span, y(1,:), 'LineWidth', line_width, 'Color', 'b', 'DisplayName', 'x');
xlabel('时间 (s)', 'FontSize', font_size);
ylabel('x (m)', 'FontSize', font_size);
title('状态输出 (x)', 'FontSize', font_size+2);
grid on;
legend('show');

subplot(3,1,2);
plot(0:dt:span, y(2,:), 'LineWidth', line_width, 'Color', 'r', 'DisplayName', 'y');
xlabel('时间 (s)', 'FontSize', font_size);
ylabel('y (m)', 'FontSize', font_size);
title('状态输出 (y)', 'FontSize', font_size+2);
grid on;
legend('show');

subplot(3,1,3);
plot(0:dt:span, y(3,:), 'LineWidth', line_width, 'Color', 'g', 'DisplayName', '\theta');
xlabel('时间 (s)', 'FontSize', font_size);
ylabel('\theta (rad)', 'FontSize', font_size);
title('状态输出 (\theta)', 'FontSize', font_size+2);
grid on;
legend('show');

% 3. 控制量（u1, u2）
figure('Name', '控制量', 'Position', [100, 100, 800, 400]);
subplot(2,1,1);
plot(0:dt:span, u(1,:), 'LineWidth', line_width, 'Color', 'b', 'DisplayName', 'u_1');
xlabel('时间 (s)', 'FontSize', font_size);
ylabel('控制量 u_1', 'FontSize', font_size);
title('控制量 (u_1)', 'FontSize', font_size+2);
grid on;
legend('show');

subplot(2,1,2);
plot(0:dt:span, u(2,:), 'LineWidth', line_width, 'Color', 'r', 'DisplayName', 'u_2');
xlabel('时间 (s)', 'FontSize', font_size);
ylabel('控制量 u_2', 'FontSize', font_size);
title('控制量 (u_2)', 'FontSize', font_size+2);
grid on;
legend('show');

% 4. 小车轨迹（x-y 平面）
figure('Name', '小车轨迹', 'Position', [100, 100, 600, 600]);
plot(x(1,:), x(2,:), 'LineWidth', line_width, 'Color', 'b', 'DisplayName', '轨迹');
xlabel('x (m)', 'FontSize', font_size);
ylabel('y (m)', 'FontSize', font_size);
title('小车轨迹 (x-y 平面)', 'FontSize', font_size+2);
grid on;
axis equal; % 保持 x-y 比例一致
legend('show');