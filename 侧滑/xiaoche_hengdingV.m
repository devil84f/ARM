clear all; clc; close all;

% 时间参数
dt = 0.1;
T = 10;                      % 总时间
t = 0:dt:T;
N = length(t);

% 系统矩阵
A = [1 0 0 0 0;
     0 1 0 0 0;
     0 0 1 0 0;
     0 0 0 0 0;
     0 0 0 0 0];

% 初始化
x_real = zeros(5,N);     % 真正的状态
x_hat = zeros(5,N);      % 观测器估计
y = zeros(3,N);          % 观测输出
u = zeros(2,N);          % 控制输入

% 初始状态
x_real(:,1) = [0; 0; 0; 0; 0];
x_hat(:,1)  = [0; 0; 0; 0; 0];

% 观测矩阵
C = [1 0 0 0 0;
     0 1 0 0 0;
     0 0 1 0 0;
     0 0 0 0 0;
     0 0 0 0 0];

% 观测器增益 L（可调）
% 你可以使用 pole placement 或 LQR 观测器设计
L = place(A', C', [0.3 0.3 0.3 0.1 0.1])';   % Luenberger 增益

for k = 1:N-1
    % 控制输入：正弦变化
    u(:,k) = [sin(0.5*t(k)); 0.2*cos(0.5*t(k))];
    
    % B 矩阵是状态相关的（x(3) 是 yaw）
    B = [dt*cos(x_real(3,k)) 0;
         dt*sin(x_real(3,k)) 0;
         0 dt;
         1 0;
         0 1];
    
    % 系统更新（真实状态）
    x_real(:,k+1) = A * x_real(:,k) + B * u(:,k);
    
    % 传感器输出
    y(:,k) = C * x_real(:,k);
    
    % 观测器更新
    % 注意此处 B 和 A 是线性的近似，我们用当前估计 yaw 更新 B_hat
    B_hat = [dt*cos(x_hat(3,k)) 0;
             dt*sin(x_hat(3,k)) 0;
             0 dt;
             1 0;
             0 1];

    x_hat(:,k+1) = A * x_hat(:,k) + B_hat * u(:,k) + L * (y(:,k) - C * x_hat(:,k));
end

% 画出估计结果
figure;
subplot(2,1,1);
plot(t, x_real(4,:), 'b', t, x_hat(4,:), 'r--');
legend('v真实值','v估计值');
xlabel('时间 (s)');
ylabel('v (m/s)');

subplot(2,1,2);
plot(t, x_real(5,:), 'b', t, x_hat(5,:), 'r--');
legend('w真实值','w估计值');
xlabel('时间 (s)');
ylabel('w (rad/s)');

