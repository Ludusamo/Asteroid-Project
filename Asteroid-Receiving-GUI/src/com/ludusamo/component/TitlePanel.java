package com.ludusamo.component;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GridBagLayout;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import javax.swing.JLabel;
import javax.swing.JPanel;

import com.ludusamo.util.Util;

public class TitlePanel extends JPanel {
	public static final int MIN_FONT_SIZE = 24;
	public static final int MAX_FONT_SIZE = 30;

	private String title;
	JLabel titleLabel;

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

	public TitlePanel(String title) {
		super(new GridBagLayout());
		titleLabel = new JLabel(title);
		this.title = title;
		titleLabel.setFont(MainComponent.font.deriveFont(12));
		titleLabel.setForeground(Color.white);

		add(titleLabel);

		validate();
		setVisible(true);

		addComponentListener(new ComponentAdapter() {
			public void componentResized(ComponentEvent e) {
				adaptLabelFont(titleLabel);
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
}
