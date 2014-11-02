package com.ludusamo.util;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.image.BufferedImage;

public class Util {
	public static void drawBackground(Component c, BufferedImage bg, Graphics g) {
		// Corners
		g.drawImage(bg, 0, 0, 13, 13, 0, 0, 13, 13, null);
		g.drawImage(bg, c.getWidth() - 13, 0, c.getWidth(), 13, 26, 0, 39, 13,
				null);
		g.drawImage(bg, 0, c.getHeight() - 13, 13, c.getHeight(), 0, 26, 13,
				39, null);
		g.drawImage(bg, c.getWidth() - 13, c.getHeight() - 13, c.getWidth(),
				c.getHeight(), 26, 26, 39, 39, null);
		// Sides
		g.drawImage(bg, 13, 0, c.getWidth() - 13, 13, 13, 0, 26, 13, null);
		g.drawImage(bg, 0, 13, 13, c.getHeight() - 13, 0, 13, 13, 26, null);
		g.drawImage(bg, c.getWidth() - 13, 13, c.getWidth(),
				c.getHeight() - 13, 26, 13, 39, 26, null);
		g.drawImage(bg, 13, c.getHeight() - 13, c.getWidth() - 13,
				c.getHeight(), 13, 26, 26, 39, null);
		// Center
		g.drawImage(bg, 13, 13, c.getWidth() - 13, c.getHeight() - 13, 13, 13,
				26, 26, null);
	}
	
	public static void drawTitle(Component c, BufferedImage title, Graphics g) {
		g.drawImage(title, 0, 0, null);
	}
}
