package com.ludusamo.util;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import com.ludusamo.component.DataPanel;
import com.ludusamo.component.ChartPanel;

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
	
	public static void saveData(String fileName, ChartPanel table, DataPanel data) {
		PrintWriter writer = null;
		try {
			writer = new PrintWriter("data/" + fileName, "UTF-8");
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		ArrayList<Vector2f> points = table.getPoints();
		writer.println(data.getTimeElapsed());
		writer.println(data.getSlope());
		writer.println(data.getNumDataPoints());
		for (int i = 0; i < points.size(); i++) {
			writer.println((i + 1) + ": {" + points.get(i).x  + ", " + points.get(i).y + "}");
		}
		writer.close();
	}
}
