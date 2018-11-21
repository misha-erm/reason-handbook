type state = {count: int};

type action =
  | Click;

let component = ReasonReact.reducerComponent("Counter");

let make = _children => {
  ...component,
  initialState: () => {count: 0},
  reducer: (action, state) =>
    switch (action) {
    | Click => ReasonReact.Update({count: state.count + 1})
    },

  render: self => {
    let message =
      "You've clicked this " ++ string_of_int(self.state.count) ++ " times(s)";
    <div>
      <button onClick={_event => self.send(Click)}>
        {ReasonReact.string(message)}
      </button>
    </div>;
  },
};