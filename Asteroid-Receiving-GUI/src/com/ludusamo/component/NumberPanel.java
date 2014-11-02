package com.ludusamo.component;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.image.BufferedImage;

import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.ludusamo.util.Util;

public class NumberPanel extends JPanel {
	public static final int MIN_FONT_SIZE = 24;
	public static final int MAX_FONT_SIZE = 30;

	private String title;
	JLabel[] labels;

	private Dimension minimumDimension;

	Graphics g;

	private void changeFontSize(JLabel l, int size) {
		Font f = l.getFont();
		l.setFont(f.deriveFont(f.getStyle(), size));
	}

	protected void adaptLabelFont(JLabel l) {
		if (g == null) {
			// System.out.println("Not working.");
			return;
		}

		float newSize = (minimumDimension.width / getSize().width)
				* (MAX_FONT_SIZE - MIN_FONT_SIZE);
		newSize = MAX_FONT_SIZE - newSize;

		changeFontSize(l, (int) newSize);

		l.repaint();
		revalidate();
	}

	public NumberPanel(String title) {
		labels = new JLabel[2];
		this.title = title;

		setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

		labels[0] = new JLabel(title);

		labels[1] = new JLabel("-");

		for (int i = 0; i < labels.length; i++) {
			labels[i].setAlignmentX(CENTER_ALIGNMENT);
			labels[i].setAlignmentY(CENTER_ALIGNMENT);
			labels[i].setFont(MainComponent.font.deriveFont(12));
			labels[i].setForeground(Color.white);
			add(labels[i]);
		}

		validate();
		setVisible(true);

		addComponentListener(new ComponentAdapter() {
			public void componentResized(ComponentEvent e) {
				for (int i = 0; i < labels.length; i++) {
					adaptLabelFont(labels[i]);
				}
			}
		});
	}

	@SuppressWarnings("deprecation")
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		this.g = g;

		Util.drawBackground(this, MainComponent.backgroundImg, g);
	}

	public void setMinimumDimension() {
		minimumDimension = getSize();
	}

	public void setNumLabel(String num) {
		labels[1].setText(num);
	}

	public float getValue() {
		if (labels[1].getText() == "-")
			return -1;
		return Float.parseFloat(labels[1].getText());
	}
}
