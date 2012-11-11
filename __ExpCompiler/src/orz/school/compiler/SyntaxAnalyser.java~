package orz.school.compiler;

import java.awt.TextArea;
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JTextArea;


class CompilerInitializer implements CompilerCons{
    static Map<String, String> getID_STRMap() {
    Map<String, String> result = new HashMap<String, String>();
    result.put(String.valueOf(PLUS), "+");
    result.put(String.valueOf(MINUS), "-");
    result.put(String.valueOf(MUL), "*");
    result.put(String.valueOf(DIV), "/");
    result.put(String.valueOf(L_BRAC), "(");
    result.put(String.valueOf(R_BRAC), ")");
    result.put(String.valueOf(EQUAL), "=");
    result.put(String.valueOf(ID), "I");
    result.put(String.valueOf(INT), "N");
    result.put(String.valueOf(FLOAT), "N");
    result.put("F", "F");
    result.put("S", "S");
    result.put("T", "T");
    result.put("E", "E");
    result.put("#", "#");
   return result;
    }
    

	static Map<String, String> [] getACTION(String actionFile) {
    	HashMap<String, String> [] result = null;
    	ArrayList<String> text = new ArrayList<String>();
		try {
			BufferedReader file = new BufferedReader(new FileReader(actionFile));
			String line = null;
			while((line = file.readLine()) != null )
				text.add(line);
			file.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		result = (HashMap<String, String>[])new HashMap[text.size()];
		for(int i=0; i<text.size(); i++) {
			String line = text.get(i);
			HashMap<String, String> temp = new HashMap<String, String>();
			int index1 = 0;
			int index2 = 0;
			int count = 0;
			while (count<SYMBOL.length) {
				index2 = line.indexOf(',',index1);  // 末尾需要,
				if(!line.substring(index1,index2).equals(""))
					temp.put(SYMBOL[count],line.substring(index1,index2));
				index1=index2+1;
				count++;
				}
			result[i]=temp;
		}
    	return result;
    }
	
	static Map<String, String> [] getGOTO(String gotoFile) {
    	HashMap<String, String> [] result = null;
    	ArrayList<String> text = new ArrayList<String>();
		try {
			BufferedReader file = new BufferedReader(new FileReader(gotoFile));
			String line = null;
			while((line = file.readLine()) != null )
				text.add(line);
			file.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		result = (HashMap<String, String>[])new HashMap[text.size()];
		for(int i=0; i<text.size(); i++) {
			String line = text.get(i);
			HashMap<String, String> temp = new HashMap<String, String>();
			int index1 = 0;
			int index2 = 0;
			int count = 0;
			while (count<NONTERMIAL.length) {
				index2 = line.indexOf(',',index1);  // 末尾需要,
				if(!line.substring(index1,index2).equals(""))
					temp.put(NONTERMIAL[count],line.substring(index1,index2));
				index1=index2+1;
				count++;
			    }
			result[i]=temp;
		}
    	return result;
	}
}

interface CompilerCons {
	
	public static final String [] PRODUCTIONS = {
		"S:I=E",
		"E:E+T",
		"E:E-T",
		"E:T",
		"T:T*F",
		"T:T/F",
		"T:F",
		"F:N",
		"F:I",
		"F:(E)" };
	
	static final int  PLUS = 0;
	static final int  MINUS = 1;
	static final int  MUL = 2;
	static final int  DIV = 3;
	static final int  L_BRAC = 4;
	static final int  R_BRAC = 5;
	static final int  EQUAL = 6;
	static final int  INT = 7;
	static final int  FLOAT = 8;
	static final int  ID = 9;
	static final int  SEMICO = 10;
	static final int  COMMA = 11;
	static final int  UNDER_SC = 12;
	static final int  KEY_INT = 22;
	static final int  KEY_FLOAT = 23;
	
	static final String SYMBOL[] = { "I","+", "-", "*", "/", "(", ")", "=" ,"N", "#"};
	static final String NONTERMIAL[] = {"S","E","T","F"};
    public static final Map<String, String> ID_STR = CompilerInitializer.getID_STRMap();
	public static final Map<String, String> [] ACTION = CompilerInitializer.getACTION("actionTable");
	public static final Map<String, String> [] GOTO =  CompilerInitializer.getGOTO("gotoTable");
	
}

	class Token {
		final String code;
		final String symbol;
		final String type;
		String value;
		public Token(String c, String s, String v, String t) {
			code = c; 
			symbol = s;
			type = t;
			value = v;
		}
	}
	
	class SymEntry {
		String value;
		String type;
		public SymEntry(String t,String v) {
			type=t;
			value=v;
		}
		public void setValue(String v) {
			value = v;
		}
		public String getType() {
			return type;
		}
		public String getValue() {
			return value;
		}
	}

public class SyntaxAnalyser implements CompilerCons {

	static void showTable()
	{
		for(int i=0; i<ACTION.length; i++) {
			for(String s:ACTION[i].keySet())
				System.out.print(s+":"+ACTION[i].get(s)+"  ");
			System.out.println();		
		}
		
		for(int i=0; i<GOTO.length; i++) {
			for(String s:GOTO[i].keySet())
				System.out.print(s+":"+GOTO[i].get(s)+"  ");
			System.out.println();		
		}
	}
	

	
	public static String analysis(JTextArea tArea, String inputfile, HashMap<String, SymEntry> symboleTable)
	{
		//showTable();
		String result = "";
		ArrayList<String> stateStack = new ArrayList<String>();
		ArrayList<Token> symboleStack = new ArrayList<Token>();
		ArrayList<String> tokenStream = new ArrayList<String>();
		try {
			BufferedReader file = new BufferedReader(new FileReader(inputfile));
			String line = null;
			while((line = file.readLine()) != null )
				tokenStream.add(line);
			file.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		tokenStream.add("#");
		System.out.println(tokenStream.size());
		Token [] tokens = new Token[tokenStream.size()];
		for(int i=0; i<tokenStream.size(); i++) 
			tokens[i] = getToken(tokenStream.get(i), symboleTable);

		int pos=0;
		String s = "";
		String next = "";
	    Token a = null;
	    
	    stateStack.add("0");
	    a = tokens[pos++];
	    int count = 0;
		while(true) {
			s=stateStack.get(stateStack.size()-1);  // current top state
			if(s==null)	{
				System.out.println("???");
			}
			System.out.println(s+"#"+stateStack.size());
			if(!ID_STR.get(a.code).equals(a.symbol)) {
				next = ACTION[Integer.parseInt(s)].get("I");
			}
			else 
				next = ACTION[Integer.parseInt(s)].get(a.symbol);
			
			if(next == null) {
				System.out.println(s);
				System.out.println(a.symbol);
				System.out.println("Error: NULL ACTION");
			}
			if( next.startsWith("s")) {  // shift t
				symboleStack.add(a);
				System.out.println(next.substring(1)+" add state");
				stateStack.add(next.substring(1));  // skip 's'
				a = tokens[pos++]; // next symbol
			} else if ( next.startsWith("r")) {  // reduce
				String prdc = PRODUCTIONS[Integer.parseInt(next.substring(1))]; // skip 'r'
				String head = prdc.substring(0,1);
				char [] body = prdc.substring(2).toCharArray();
				result+=(prdc+"\n");
				tArea.setText(result);
				String value = null;
				String type = null;
				String symbol = null;
				//Token token = null;
				if(Integer.parseInt(next.substring(1)) == 0) {
					// "S:I=E"
					String sym = symboleStack.get(symboleStack.size()-3).symbol;
					value = symboleTable.get(sym).value = symboleStack.get(symboleStack.size()-1).value;
					type = symboleTable.get(sym).type;  // ID has its type
					System.out.println(symboleTable.get(sym).type);
					symboleTable.get(sym).type = symboleStack.get(symboleStack.size()-1).type;
					result+=( sym +"=" + symboleStack.get(symboleStack.size()-1).symbol);
					
				}
				else if(Integer.parseInt(next.substring(1)) == 1) {
					// "E:E+T"
					Token E = symboleStack.get(symboleStack.size()-3);
					Token T = symboleStack.get(symboleStack.size()-1);
					if(E.type.equals("float") || T.type.equals("float")) {
						value = String.valueOf(Float.parseFloat(E.value) +
											   Float.parseFloat(T.value));
						type = "float";  
					}
					else 
					{
						value = String.valueOf(Integer.parseInt(E.value) +
								   Integer.parseInt(T.value));
						type = "int";  
					}
					
					symbol = "t"+count;
					count++;
					
					result+=( symbol +"=" + E.symbol+" + "+ T.symbol+"\n\n");
					tArea.setText(result);
					//token = new Token("",head,value,type);
				}
				else if(Integer.parseInt(next.substring(1)) == 2) {
					// "E:E-T"
					Token E = symboleStack.get(symboleStack.size()-3);
					Token T = symboleStack.get(symboleStack.size()-1);
					if(E.type.equals("float") || T.type.equals("float")) {
						value = String.valueOf(Float.parseFloat(E.value) -
											   Float.parseFloat(T.value));
						type = "float";  
					}
					else 
					{
						value = String.valueOf(Integer.parseInt(E.value) -
								   Integer.parseInt(T.value));
						type = "int";  // ID has its type
					}
					symbol = "t"+count;
					count++;
					
					result+=( symbol +"=" + E.symbol+" - "+ T.symbol+"\n\n");
					tArea.setText(result);
				}
				else if(Integer.parseInt(next.substring(1)) == 3) {
					//"E:T"
					Token T = symboleStack.get(symboleStack.size()-1);
					value = T.value;
					type = T.type;
					symbol = T.symbol;
				}
				else if(Integer.parseInt(next.substring(1)) == 4) {
					//"T:T*F"
					Token T = symboleStack.get(symboleStack.size()-3);
					Token F = symboleStack.get(symboleStack.size()-1);
					if(F.type.equals("float") || T.type.equals("float")) {
						value = String.valueOf(Float.parseFloat(T.value) *
											   Float.parseFloat(F.value));
						type = "float"; 
					}
					else 
					{
						value = String.valueOf(Integer.parseInt(T.value) *
								   Integer.parseInt(F.value));
						type = "int";  // ID has its type
					}
					symbol = "t"+count;
					count++;
					
					result+=( symbol +"=" + T.symbol+" * "+ F.symbol+"\n\n");
					tArea.setText(result);
				}
				else if(Integer.parseInt(next.substring(1)) == 5) {
					// "T:T/F"
					Token T = symboleStack.get(symboleStack.size()-3);
					Token F = symboleStack.get(symboleStack.size()-1);
					if(F.type.equals("float") || T.type.equals("float")) {
						value = String.valueOf(Float.parseFloat(T.value) /
											   Float.parseFloat(F.value));
						type = "float";  
					}
					else 
					{
						value = String.valueOf(Integer.parseInt(T.value) /
								   Integer.parseInt(F.value));
						type = "int";  // ID has its type
					}
					symbol = "t"+count;
					count++;
					
					result+=( symbol +"=" + T.symbol+" / "+ F.symbol+"\n");
					tArea.setText(result);				
				}
				else if(Integer.parseInt(next.substring(1)) == 6) {
					// T:F
					Token F = symboleStack.get(symboleStack.size()-1);
					value = F.value;
					type = F.type;
					symbol = F.symbol;
				}
				else if(Integer.parseInt(next.substring(1)) == 7) {
					// F:N
					Token N = symboleStack.get(symboleStack.size()-1);
					value = N.value;
					type = N.type;	
					symbol = N.value; // value as symbol
				}
				else if(Integer.parseInt(next.substring(1)) == 8) {
					// F:I
					Token I = symboleStack.get(symboleStack.size()-1);
					System.out.println(I.symbol);
					value = symboleTable.get(I.symbol).value;
					type = symboleTable.get(I.symbol).type;	
					symbol = I.symbol;
				}
				else if(Integer.parseInt(next.substring(1)) == 9) {
					// F:(E)
					Token E = symboleStack.get(symboleStack.size()-2);
					value = E.value;
					type = E.type;	
					symbol = E.symbol;
				}
				
				for(int i=0; i<body.length; i++) { // pop stack
					/*
					if( !ID_STR.get(symboleStack.get(symboleStack.size()-1).code).equals(symboleStack.get(symboleStack.size()-1).code) 
						&& !"I".equals(""+body[body.length-i-1]))
					{ // Check Error
						System.out.println("Error Reduce!!!"); 
					}
					else if(!ID_STR.get(symboleStack.get(symboleStack.size()-1).code).equals(""+body[body.length-i-1]) )
					{
						System.out.println("Error Reduce!!!"); 
					}
					*/
					stateStack.remove(stateStack.size()-1); 
					symboleStack.remove(symboleStack.size()-1);
				}
				s=stateStack.get(stateStack.size()-1);
				if(GOTO[Integer.parseInt(s)].get(head)==null) {
					System.err.println("WTF");
				}
				stateStack.add(GOTO[Integer.parseInt(s)].get(head));
				symboleStack.add(new Token("",symbol,value,type)); 
				
				//TODO Get Value/ What about type?
				
			} else if (next.equals("acc"))
					break;
			else 
				System.err.println("Error ACTION Type");
		}
		
		return result;
	}
	
	private static Token getToken(String token, HashMap<String, SymEntry> symboleTable) {
		if(symboleTable.get(getSymbole(token)) != null)
		return new Token(getID(token), 
				  		 getSymbole(token),
				  		 symboleTable.get(getSymbole(token)).getValue(),
				  		 getType(token));
		else if(getID(token).equals(String.valueOf(INT)) || 
				getID(token).equals(String.valueOf(FLOAT)))
		return new Token(getID(token),
						 getSymbole(token),
						 getValue(token),
						 getType(token));
		else
		return new Token(getID(token), 
			  		 	 getSymbole(token),
			  		 	 "",
			  		 	 getType(token));
	}



	private static String getType(String token) {
		if(getID(token).equals(String.valueOf(INT)))
			return "int";
		else if(getID(token).equals(String.valueOf(FLOAT)))
			return "float";
		else 
			return "";
	}


	private static String getValue(String token) {
		return token.substring(token.indexOf("\t")+1, token.length());
	}
	private static String getID(String token) {
	//	System.out.println(token+"@");
		if(token.indexOf("\t")<0)
			return token;
		return token.substring(0, token.indexOf("\t"));
	}
	
	private static String getSymbole(String token) {
		if(token.equals("#"))
			return token;
		else if(getID(token).equals(String.valueOf(ID)))
			return token.substring(token.lastIndexOf("\t")+1,token.length());
		else
			return ID_STR.get(getID(token));
	}
}

