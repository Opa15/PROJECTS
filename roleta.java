import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Random;

public class roleta extends JFrame {
    private static final int NUMERO_SEGMENTOS = 10;
    private static final int TAMANHO_RODA = 300;
    private int resultadoAtual = -1;
    private Timer timer;
    private Random random;

    public roleta() {
        random = new Random();
        setTitle("Roleta Gráfica");
        setSize(400, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        JButton botaoGirar = new JButton("Girar Roleta");
        botaoGirar.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                iniciarGiro();
            }
        });

        add(botaoGirar, BorderLayout.SOUTH);
        setVisible(true);
    }

    // Iniciar o giro da roleta
    private void iniciarGiro() {
        if (timer != null && timer.isRunning()) {
            return;
        }

        timer = new Timer(100, new ActionListener() {
            private int passos = 20; // Número de passos no giro

            @Override
            public void actionPerformed(ActionEvent e) {
                resultadoAtual = random.nextInt(NUMERO_SEGMENTOS);
                repaint();
                passos--;

                if (passos == 0) {
                    timer.stop();
                    mostrarResultado();
                }
            }
        });

        timer.start();
    }

    // Mostrar o resultado após o giro
    private void mostrarResultado() {
        int porcentagemGanho = 30; // 30% de chance de ganhar
        int sorteio = random.nextInt(100);

        if (sorteio < porcentagemGanho) {
            JOptionPane.showMessageDialog(this, "Você ganhou!");
        } else {
            JOptionPane.showMessageDialog(this, "Você perdeu!");
        }
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);

        Graphics2D g2d = (Graphics2D) g;
        int centroX = getWidth() / 2;
        int centroY = getHeight() / 2 - 50;

        int raio = TAMANHO_RODA / 2;
        int angulo = 360 / NUMERO_SEGMENTOS;

        for (int i = 0; i < NUMERO_SEGMENTOS; i++) {
            // Definir a cor alternada para os segmentos
            if (i % 2 == 0) {
                g2d.setColor(Color.LIGHT_GRAY);
            } else {
                g2d.setColor(Color.WHITE);
            }

            g2d.fillArc(centroX - raio, centroY - raio, TAMANHO_RODA, TAMANHO_RODA, i * angulo, angulo);

            // Desenhar o número no segmento
            g2d.setColor(Color.BLACK);
            int x = centroX + (int) (raio * 0.7 * Math.cos(Math.toRadians((i * angulo) + angulo / 2)));
            int y = centroY + (int) (raio * 0.7 * Math.sin(Math.toRadians((i * angulo) + angulo / 2)));
            g2d.drawString(String.valueOf(i), x - 5, y + 5);
        }

        // Destacar o segmento atual
        if (resultadoAtual != -1) {
            g2d.setColor(Color.RED);
            g2d.drawLine(centroX, centroY,
                    centroX + (int) (raio * Math.cos(Math.toRadians((resultadoAtual * angulo) + angulo / 2))),
                    centroY + (int) (raio * Math.sin(Math.toRadians((resultadoAtual * angulo) + angulo / 2))));
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new roleta();
        });
    
}